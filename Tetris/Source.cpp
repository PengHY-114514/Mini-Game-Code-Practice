#include <iostream>
#include <windows.h>
#include <thread>
#include <list> 

using namespace std;





wstring tetromino[7];
const int tetrominoWidth = 4;

int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;


const int nScreenWidth = 30;
const int nScreenHeight = 20;


wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];


HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
DWORD dwBytesWritten = 0;


int nCurrentPiece = 5;
int nCurrentRotation = 0;
int nCurrentX = nFieldWidth / 2;
int nCurrentY = 0;

string intro[4] = { "UP - Rotate",
				"L - Move Left",
				"R - Move Right",
				"Down - Drop " };




int bgm();
int  Rotate(int px, int py, int r, int pw = tetrominoWidth)
{
	switch (r % 4)
	{
	case 0:		//0
		return py * pw + px;
	case 1:		//90
		return pw * (pw - 1) - px * 4 + py;
	case 2:		//180
		return pw * pw - py * 4 - px - 1;
	case 3:		//270
		return pw - py + px * 4 - 1;
	}

	return 0;
}


bool DoesPieceFit(int nTeromino, int nRotation, int nPosX, int nPosY)
{
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			//Get  Index into piece
			int pi = Rotate(px, py, nRotation);

			//Get  Index into field
			int fi = Rotate(nPosX + px, nPosY + py, 0, nFieldWidth);

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					if (tetromino[nTeromino][pi] == L'X' && pField[fi] != 0)
					{
						return false;//fail on first hit
					}
				}
			}


		}
	}
	



	return true;
}

void Update()
{


	//Draw Field
	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			screen[Rotate(x + 2, y + 2, 0, nScreenWidth)] = L" HHHHHHH=*"[pField[Rotate(x, y, 0, nFieldWidth)]];
		}
	}

	//Draw intro
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < intro[i].length(); j++)
		{
			screen[Rotate(16 + j, 2 + 2*i, 0, nScreenWidth)] = intro[i][j];
			//screen[Rotate(x + 2, y + 2, 0, nScreenWidth)] = L" HHHHHHH=*"[pField[Rotate(x, y, 0, nFieldWidth)]];
		}
	}



	//Draw Current Piece
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
			{
				screen[Rotate(nCurrentX + px + 2, nCurrentY + py + 2, 0, nScreenWidth)] = 'H';// nCurrentPiece + 65;
				

			}


		}
	}




	//Display Frame
	WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth*nScreenHeight, { 0, 0 }, &dwBytesWritten);//ensure u have set unicode enabled

}


list<int> vLine;

int main()
{
	int num = 0;
	thread tbgm(bgm);
	//();
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");

	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");

	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");

	tetromino[6].append(L"....");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");

	pField = new unsigned char[nFieldWidth*nFieldHeight];//creat play field
	for (int x = 0; x < nFieldWidth; x++)// board boundary
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			int i = Rotate(x, y, 0, nFieldWidth);

			if (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1)
				pField[i] = 9;
			else
				pField[i] = 0;
		}
	}


	
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++)
	{
		screen[i] = L' ';
	}
	
	
	



	
/////////////////////////////窗口设置//////////////////////////////////////////
	//SMALL_RECT wrt = { 0, 0, short(nScreenWidth - 1), short(nScreenHeight - 1) };
	SetConsoleActiveScreenBuffer(hConsole);



	SMALL_RECT wrt = { 0, 0, nScreenWidth-1, nScreenHeight-1 };
	SetConsoleWindowInfo(hConsole, TRUE, &wrt);


	COORD coord = { nScreenWidth-1, nScreenHeight-1 };
	SetConsoleScreenBufferSize(hConsole, coord); // 设置缓冲尺寸



	 wrt = { 0, 0, nScreenWidth - 1, nScreenHeight - 1 };
	SetConsoleWindowInfo(GetConsoleWindow(), TRUE, &wrt);


	 coord = { nScreenWidth - 1, nScreenHeight - 1 };
	SetConsoleScreenBufferSize(GetConsoleWindow(), coord); // 设置缓冲尺寸


	
	SetWindowLongPtrA(
		GetConsoleWindow(),
		GWL_STYLE,
		GetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE)
		& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX
	);

	


	SetConsoleTitle(LPCWSTR(L"HY's Tetris"));


	////////////////////////////////////////////////////////////////////////////////	

	bool bGameOver = false;




	bool bKey[4];
	bool bRotateHold = false;

	int nSpeed = 20;
	int nSpeedCounter = 0;
	bool bForceDown = false;




	while (!bGameOver)
	{	
		//Game Timing ==================================
		this_thread::sleep_for(50ms);
		nSpeedCounter++;

		if (nSpeedCounter == nSpeed)
		{
			bForceDown = true;
		}




		//Input ==================================
		for (int k = 0; k < 4; k++)
		{														// R   L   D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x26"[k]))) != 0;

		}




		//Game Logic ============================

		if (bKey[1])
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY))
			{
				nCurrentX = nCurrentX - 1;
			}
		}

		if (bKey[0])
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY))
			{
				nCurrentX = nCurrentX + 1;
			}
		}
		if (bKey[2])
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1 ))
			{
				nCurrentY = nCurrentY + 1;
			}
		}

		if (bKey[3]&&!bRotateHold)
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation+1, nCurrentX, nCurrentY))
			{
				nCurrentRotation = nCurrentRotation + 1;
				
			}
			bRotateHold = true;
		}

		if (!bKey[3])
		{
			bRotateHold = false;
		}


		if (bForceDown)
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
			{
				nCurrentY = nCurrentY + 1;
			}
			else
			{
				//lock the cur piece in the field
				for (int px = 0; px < 4; px++)
				{
					for (int py = 0; py < 4; py++)
					{
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
						{
							pField[Rotate(nCurrentX + px, nCurrentY + py, 0, nFieldWidth)] = nCurrentPiece + 1;
						}
					}
				}


				//choose next piece

				

				//check have we got any lines

				for (int py = 0; py < 4; py++)
				{
					if (nCurrentY + py < nFieldHeight - 1)
					{
						bool bLine = true;
						for (int px = 1; px < nFieldWidth-1; px++)
						{
							bLine &= (pField[Rotate(px, nCurrentY + py, 0, nFieldWidth)]) != 0;
						}




						if (bLine)
						{
							vLine.push_back(nCurrentY + py);	
							
						}








					}
				}

				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;

				while (!vLine.empty())
				{

					for (int px = 1; px < nFieldWidth - 1; px++)
					{
						pField[Rotate(px, vLine.front(), 0, nFieldWidth)] = 8;//==
						Update();
						this_thread::sleep_for(20ms);
					}

					this_thread::sleep_for(200ms);
					for (int cy = vLine.front(); cy >= 2; cy--)
					{
					
						for (int px = 1; px < nFieldWidth - 1; px++)
						{
							pField[Rotate(px, cy, 0, nFieldWidth)] = pField[Rotate(px, cy - 1, 0, nFieldWidth)];//==
						}
						
					}
					Update();
					
					
					vLine.pop_front();
				}


				

				//if piece does fit
				bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);



			}
			bForceDown = false;
			nSpeedCounter = 0;
		}




		//Render Output ============================
	
		//Draw Scene
		/*for (int x = 0; x < nScreenWidth; x++)
		{
			for (int y = 0; y < nScreenHeight; y++)
			{
				screen[Rotate(x,y,0, nScreenWidth)] = L".ABCDEFG=#"[0];
			}
		}*/
		Update();

		
		
	
	}

	CloseHandle(hConsole);

	cout << "寄" << endl;
	system("pause");
	tbgm.detach();


	//while(1)
	//{ 
	////Draw Scene
	//	for (int x = 0; x < nScreenWidth; x++)
	//	{
	//		for (int y = 0; y < nScreenHeight; y++)
	//		{
	//			screen[Rotate(x,y,0, nScreenWidth)] = L".ABCDEFG=#"[0];
	//		}
	//	}

	//WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth*nScreenHeight, { 0, 0 }, &dwBytesWritten);//ensure u have set unicode enabled
	//}
	//




	return 0;
}