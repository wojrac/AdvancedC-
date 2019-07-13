#include"functions.h"
#include<boost\filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include<filesystem>
#include<ctime>

using namespace boost::filesystem;
namespace fs= std::experimental::filesystem;
//using namespace sf;
int coordinateToNumber(int coordinateX, int coordinateY, shared_ptr<Cell>& arr, int sX, int sY)
{
	int i = 0;
	while (!(arr.get()[i].getCoordinateX() == coordinateX && arr.get()[i].getCoordinateY() == coordinateY) && i<sX*sY)
	{

		i++;
	}
	return  i;
}
vector<string> decompressVec(vector<vector<char>> &vec)
{
	vector<string> vecSTR;
	vector<string> vecSTR2;
	vecSTR.reserve(vec.size());
	int ich, counter;
	int sizeVect;
	vector<int> tmp;
	int cnt = 0;
	int is;
	for (auto const & v : vec)
		vecSTR.emplace_back(v.begin(), v.end());  //vector char[] ->vector string
	for (int i = 0; i < vecSTR.size(); i++)
	{
		sizeVect = vecSTR[i].size();
		for (int j = 0; j <sizeVect; j = j++)
		{
			//counter = 0;
			if (isdigit(vecSTR[i][j]))
			{
				if (j == sizeVect - 1)
				{
					vecSTR[i].push_back(vecSTR[i][j]);
					is = stoi(string(1, vecSTR[i][j]));
					for (int l = 0; l < is - 1; l++)
					{
						tmp.push_back(i);
						cnt++;
					}
				}
				else
				{
					ich = stoi(string(1, vecSTR[i][j]));
					//ich = static_cast<int>(vecSTR[0][j]);

					//cout << "ich " << ich << endl;
					//cout << "vector " << vecSTR[0][j]<<endl;
					for (int k = 0; k < ich; k++)
					{
						//vecSTR[0][j] += vecSTR[0][j + 1];

						vecSTR[i].push_back(vecSTR[i][j + 1]);
						//counter++;
					}
				}

			}
			else
			{
				//wchodzi i sie nie konczy
				if (j == 0 || !isdigit(vecSTR[i][j - 1]))
				{
					vecSTR[i].push_back(vecSTR[i][j]);
				}
				else {

				}
			}
		}
		for (int l = sizeVect, m = 0; m < vecSTR[i].size(); l++, m++) {
			if (m < vecSTR[i].size() - sizeVect)
				vecSTR[i][m] = vecSTR[i][l];
			else
				vecSTR[i][m] = 0;
		}
	}
	for (int i = 0; i < tmp.size(); i++)
	{
		vecSTR.insert(vecSTR.begin() + tmp[i] + i + 1, "bbbb");
	}
	return vecSTR;
}
int main()
{
	
	
	vector<bool>memento1D;
	vector<vector<bool>> memento2D;
	unsigned int windowWidth = 1200;
	unsigned int windowHeight = 1000;
	RenderWindow window = { VideoMode{windowWidth,windowHeight} , "GAME OF LIFE" };
	//window.setFramerateLimit(60);
	CircleShape circleShape1(10.f);
	CircleShape circleShape2(10.f);
	CircleShape test(10.f);
	test.setFillColor(Color::Blue);
	circleShape1.setFillColor(Color::Green);
	circleShape2.setFillColor(Color::Red);
	Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		cout << "error loading a file " << endl;
		system("pause");
	}
	Text FPSLabel;
	Text maxRamInfo;
	Text actuallIterationLabel;
	FPSLabel.setString("ACTUAL FPS: ");
	FPSLabel.setFont(font);
	FPSLabel.setCharacterSize(15);
	FPSLabel.setPosition(windowWidth - 200, windowHeight - 50);
	FPSLabel.setFillColor(Color::White);
	maxRamInfo.setString("Max iteration: ");
	maxRamInfo.setFont(font);
	maxRamInfo.setCharacterSize(15);
	maxRamInfo.setPosition(windowWidth - 200, windowHeight - 30);
	maxRamInfo.setFillColor(Color::White);
	actuallIterationLabel.setString("Actuall iteration: ");
	actuallIterationLabel.setFont(font);
	actuallIterationLabel.setCharacterSize(15);
	actuallIterationLabel.setPosition(windowWidth - 500, windowHeight - 30);
	actuallIterationLabel.setFillColor(Color::White);
	int ButtonX = windowWidth - 200;
	//int firstButtonY = 100;
	int buttonWidth = 100;
	int buttonHeight = 40;
	Button button1(ButtonX, 100, buttonWidth, buttonHeight,font, "START(GUN)", Color(220,220,220));
	Button button10(ButtonX, 40, buttonWidth, buttonHeight, font, "START(QBS)", Color(220, 220, 220));
	Button button11(ButtonX, 140, buttonWidth, buttonHeight, font, "START(COPPHEAD)", Color(210, 210, 210));
	Button button4(ButtonX, 200, buttonWidth, buttonHeight, font, "START(glider)", Color(220,220,220));
	Button button2(ButtonX, 300, buttonWidth, buttonHeight, font, "DO TURN", Color::Yellow);
	Button button3(ButtonX, 500, buttonWidth, buttonHeight, font, "FINISH", Color(220,220,220));
	Button button5(ButtonX, 600, buttonWidth, buttonHeight, font, "MY PATTERN", Color(220,220,220));
	Button button6(ButtonX, 400, buttonWidth, buttonHeight, font, "BACK", Color::Yellow);
	Button button7(ButtonX, 0, buttonWidth, buttonHeight, font, "PLAY", Color::Blue);
	Button button8(ButtonX, 700, buttonWidth, buttonHeight, font, "STOP", Color::Blue);
	Button button9(ButtonX, 800, buttonWidth, buttonHeight, font, "SUPERBACK", Color::Blue);
	vector<string> patternFromFile;
	vector<string> patternFromFile2;
	vector<string> patternFromFile3;
	string iter,iter2,iter3;
	boost::property_tree::ptree pt;
	int counterOfPattern = 0;	
	fs::path p{ "hardPattern.rle" };
	fs::path p2{ "hardPattern2.rle" };    
	fs::path p3{ "hardPattern3.rle" };
	//if(counterOfPattern %3 ==0)
	std::ifstream ifs(p);	
	std::ifstream ifs2(p2);
	std::ifstream ifs3(p3);
	//std::ofstream ofs(p2);
	while (!ifs.eof())
	{
		ifs >> iter;
		patternFromFile.push_back(iter);
	} //czyli mam w wektorze  caly string z pliku
	while (!ifs2.eof())
	{
		ifs2 >> iter2;
		patternFromFile2.push_back(iter2);
	}
	while (!ifs3.eof())
	{
		ifs3 >> iter3;
		patternFromFile3.push_back(iter3);
	}
	//cout<<patternFromFile2.size();//0
	/*for (int i = 0; i < patternFromFile.size(); i++)
	{
		cout << patternFromFile[i] << endl;
	}*/
	//przydaloby sie poodzielac te stringi zeby kazda linijka planszy to byl oddzielny element tablicy
	vector<char> dividedByDollar, dividedByDollar22, dividedByDollar33;
	vector<vector<char>> dividedByDollar2, dividedByDollar222, dividedByDollar333;
	string concatenedPatternFromFile=" ";
	string concatenedPatternFromFile2 = " ";
	string concatenedPatternFromFile3= " ";
	for (int i = 0; i < patternFromFile.size(); i++)
	{
		concatenedPatternFromFile += patternFromFile[i];
	}
	for (int i = 0; i < patternFromFile2.size(); i++)
	{
		concatenedPatternFromFile2 += patternFromFile2[i];
	}
	for (int i = 0; i < patternFromFile3.size(); i++)
	{
		concatenedPatternFromFile3 += patternFromFile3[i];
	}
	//cout << concatenedPatternFromFile << endl;
	//cout<<"rozmiar :"<<concatenedPatternFromFile.size() << endl;
	int j = 1;
	int counter;
	for (int i = 1; concatenedPatternFromFile[i]; i=i+counter+1)
	{
		
		counter = 0;
		while(concatenedPatternFromFile[j] != '$' && concatenedPatternFromFile[j] != '!' && concatenedPatternFromFile[j])
		{
			
			dividedByDollar.push_back(concatenedPatternFromFile[j]);
			j ++ ;			
			counter++;
		}
		j++;
		dividedByDollar2.push_back(dividedByDollar);
		dividedByDollar.clear();
	}
	int k = 1;
	int counter2;
	for (int i = 1; concatenedPatternFromFile2[i]; i = i + counter2 + 1)
	{

		counter2 = 0;
		while (concatenedPatternFromFile2[k] != '$' && concatenedPatternFromFile2[k] != '!' && concatenedPatternFromFile2[k])
		{

			dividedByDollar22.push_back(concatenedPatternFromFile2[k]);
			k++;
			counter2++;
		}
		k++;
		dividedByDollar222.push_back(dividedByDollar22);
		dividedByDollar22.clear();
	}
	int l = 1;
	int counter3;
	for (int i = 1; concatenedPatternFromFile3[i]; i = i + counter3 + 1)
	{

		counter3 = 0;
		while (concatenedPatternFromFile3[l] != '$' && concatenedPatternFromFile3[l] != '!' && concatenedPatternFromFile3[l])
		{

			dividedByDollar33.push_back(concatenedPatternFromFile3[l]);
			l++;
			counter3++;
		}
		l++;
		dividedByDollar333.push_back(dividedByDollar33);
		dividedByDollar33.clear();
	}
	vector<string> decompressedVector = decompressVec(dividedByDollar2);  //wreszcie to mam!
	vector<string> decompressedVector2 = decompressVec(dividedByDollar222);
	vector<string> decompressedVector3 = decompressVec(dividedByDollar333);
	//cout << decompressedVector2.size();  // 0
	/*for (int i = 0; i < decompressedVector.size(); i++)
	{
		cout << decompressedVector[i] << endl;
	}*/
	boost::property_tree::ini_parser::read_ini("config.ini", pt);
			
	//TABLICE SASIADOW DLA SKRAJNYCH KOMOREK
	Cell neighboursWithLeftTop[5];
	Cell neighboursWithLeftDown[5];
	Cell neighboursWithRightTop[5];
	Cell neighboursWithRightDown[5];
	string sizeXs, sizeYs,cordXs[5],cordYs[5];
	int sizeX, sizeY,cordX[5],cordY[5],cordX1,cordY1, numberOfCell;
	vector<int> cordXVec,cordXVec2,cordXVec3;
	vector<int> cordYVec, cordYVec2, cordYVec3;
	try {
		
		sizeXs = pt.get<std::string>("Section1.sizeX");
		sizeYs = pt.get<std::string>("Section1.sizeY");
		sizeX = atoi(sizeXs.c_str());
		sizeY = atoi(sizeYs.c_str());
		if (sizeX <= 0 || sizeY <=0)
			throw 99;
	}
	catch (int x)
	{
		cout << "cordinates must not be less or equal than 0, ERROR NR: " << x<< endl;
		cout << "it is necesary to change this to 1 " << endl;
		if(sizeX<=0)
		sizeX = 1;
		if(sizeY<=0)
		sizeY = 1;
	}
	long long int availableRAMInBytes = 7800000000;
	int sizeOfCell = sizeof(Cell);
	int sizeOfBoard = sizeOfCell*sizeX*sizeY;
	int maxIterators;
	maxIterators = (availableRAMInBytes/sizeOfBoard);
	string maxIteratorsSTR = to_string(maxIterators);
	int actualIt = 0;
	string actualItSTR = to_string(actualIt);
	shared_ptr<Cell> myBoard(new Cell[sizeX*sizeY],default_delete<Cell[]>());
	 for (int i = 0; i <sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			myBoard.get()[j + sizeX*i].setCellDead();
			myBoard.get()[j+sizeX*i].setCoordinateX(j);
			myBoard.get()[j+sizeX*i].setCoordinateY(i);
		}
	}
	//MUSZE JAKOS TO WYPISAC
	//przed setupem
	Event event;
	
		int is;
		for (int i = 0; i < decompressedVector.size(); i++)
		{
			for (int j = 0; j < decompressedVector[i].size(); j++)
			{
				if (decompressedVector[i][j] == 'o')
				{

					cordXVec.push_back(j);
					cordYVec.push_back(i);
				}
			}
		}
		//int is;
		//cout << decompressedVector2.size();   --> 0
		for (int i = 0; i < decompressedVector2.size(); i++)
		{
			for (int j = 0; j < decompressedVector2[i].size(); j++)
			{
				if (decompressedVector2[i][j] == 'o')
				{

					cordXVec2.push_back(j);
					cordYVec2.push_back(i);
				}
			}
		}
		//int is;
		for (int i = 0; i < decompressedVector3.size(); i++)
		{
			for (int j = 0; j < decompressedVector3[i].size(); j++)
			{
				if (decompressedVector3[i][j] == 'o')
				{

					cordXVec3.push_back(j);
					cordYVec3.push_back(i);
				}
			}
		}
		
			cordXs[0] = pt.get<std::string>("Section1.cordX1");
			cordYs[0] = pt.get<std::string>("Section1.cordY1");
			cordXs[1] = pt.get<std::string>("Section1.cordX2");
			cordYs[1] = pt.get<std::string>("Section1.cordY2");
			cordXs[2] = pt.get<std::string>("Section1.cordX3");
			cordYs[2] = pt.get<std::string>("Section1.cordY3");
			cordXs[3] = pt.get<std::string>("Section1.cordX4");
			cordYs[3] = pt.get<std::string>("Section1.cordY4");
			cordXs[4] = pt.get<std::string>("Section1.cordX5");
			cordYs[4] = pt.get<std::string>("Section1.cordY5");
		
	

		unique_ptr<unique_ptr<Cell[]>[]>    neighboursWithTopRow;
		unique_ptr<unique_ptr<Cell[]>[]>    neighboursWithDownRow;
		//5->8
		unique_ptr<unique_ptr<Cell[]>[]>    neighboursWithLeftColumn;
		unique_ptr<unique_ptr<Cell[]>[]>    neighboursWithRightColumn;
		unique_ptr<unique_ptr<Cell[]>[]>    neighboursMiddle;
		unique_ptr<int[]> temporaryArray;
		unique_ptr<int[]> arrayWithSum;
		bool selectorMain = true;
		bool isStarted = false;
		int counterOfButt = 0;
		bool IsBacked = false;
		bool IsBackedAtLeastOne = false;
		bool IsDidAnyTurn = false;
		bool selectorPlay = false;
		bool isStopped = false;
		int counterOfPlayed = 0;
		int counterAfterStop = 0;
		clock_t startTime;
		clock_t endTime;
		clock_t clocksTicksTaken;
		double timeInSecond;
		double actuallFPS;
		TextField tf(8);
		tf.setPosition(30, 880);
		string FPSFromTextField;
		Text FPSTEXT;
		FPSTEXT.setPosition(60, 880);
		FPSTEXT.setFillColor(sf::Color::Black);
		FPSTEXT.setCharacterSize(14);
		FPSTEXT.setFont(font);
		string tmpFPS ;
		vector<bool> noCheatMemento1D;
		vector<vector<bool>>  noCheatMemento2D;
		bool isNoCheat = false;
		bool isSuperBacked = false;
		bool isSuperBackedAtLeastOne = false;
		bool cntToDown = false;
		while (window.isOpen())
			{
			startTime = clock();
			window.clear(Color::Black);
			window.setFramerateLimit(atoi(tmpFPS.c_str()));
				
				window.draw(button1);
				window.draw(button2);
				window.draw(button3);
				window.draw(button4);
				window.draw(button5);
				window.draw(button6);
				window.draw(button7);
				window.draw(button8);
				window.draw(button9);
				window.draw(FPSLabel);
				window.draw(maxRamInfo);
				window.draw(button10);
				window.draw(button11);
				window.draw(actuallIterationLabel);
				Text MaxItInfo;
				MaxItInfo.setString(maxIteratorsSTR);
				MaxItInfo.setFont(font);
				MaxItInfo.setCharacterSize(15);
				MaxItInfo.setPosition(windowWidth - 80, windowHeight - 30);
				MaxItInfo.setFillColor(Color::White);
				window.draw(MaxItInfo);
				Text ActualItInfo;
				actualItSTR = to_string(actualIt);
				ActualItInfo.setString(actualItSTR);
				ActualItInfo.setFont(font);
				ActualItInfo.setCharacterSize(15);
				ActualItInfo.setPosition(windowWidth - 370, windowHeight - 30);
				ActualItInfo.setFillColor(Color::White);
				window.draw(ActualItInfo);
				
			
			int id;
			if (!IsBacked && !isSuperBacked)
			{
				if (!IsBackedAtLeastOne && !isSuperBackedAtLeastOne )
				{
					
					for (int i = sizeY - 1; i >= 0; i--)
					{

						for (int j = 0; j < sizeX; j++)
						{
							
							if (myBoard.get()[j + sizeX*i].IsAlive())				//jesli nie bylo ani jednego backa
							{														//wypisuje normalnie z boarda
								window.draw(circleShape1);			
								circleShape1.setPosition(20 * j, 20 * i);
							}
							else
							{
								window.draw(circleShape2);
								circleShape2.setPosition(20 * j, 20 * i);
							}
						}
					}
				}
				else if(IsBackedAtLeastOne)
				{  //jesli byl choc jeden, wypisuje z memento
					printBoard(memento2D, circleShape1, circleShape2, sizeX, sizeY, window, counterOfButt);
				}
				else
				{
					for (int i = sizeY - 1; i >= 0; i--)
					{
						for (int j = 0; j < sizeX; j++)
						{
							if (noCheatMemento2D[counterOfPlayed - 1][j + sizeX*i] == true)
							{
								window.draw(circleShape1);
								circleShape1.setPosition(20 * j, 20 * i);
							}
							else
							{
								window.draw(circleShape2);
								circleShape2.setPosition(20 * j, 20 * i);
							}
						}
					}
				}
			}
			else if(IsBacked)
			{
				for (int i = sizeY - 1; i >= 0; i--)
				{
					for (int j = 0; j < sizeX; j++)
					{//i oczywiscie jesli byl back to tez z memento

						if (memento2D[counterOfButt - 1][j + sizeX*i] == true)
						{
							window.draw(circleShape1);
							circleShape1.setPosition(20 * j, 20 * i);
						}
						else
						{
							window.draw(circleShape2);
							circleShape2.setPosition(20 * j, 20 * i);
						}
					}
				}
			}
			else
			{
				for (int i = sizeY - 1; i >= 0; i--)
				{
					for (int j = 0; j < sizeX; j++)
					{
						if (noCheatMemento2D[counterOfPlayed - 1][j + sizeX*i] == true)
						{
							window.draw(circleShape1);
							circleShape1.setPosition(20 * j, 20 * i);
						}
						else
						{
							window.draw(circleShape2);
							circleShape2.setPosition(20 * j, 20 * i);
						}
					}
				}
			}
			if (selectorPlay && isStarted && !IsBackedAtLeastOne)
			{
				isNoCheat = true;
				solver(myBoard, neighboursWithLeftTop, neighboursWithRightTop, neighboursWithLeftDown, neighboursWithRightDown, neighboursWithTopRow, neighboursWithDownRow,
					neighboursWithLeftColumn, neighboursWithRightColumn, neighboursMiddle, arrayWithSum, temporaryArray, sizeX, sizeY, memento1D, memento2D,false,noCheatMemento1D,noCheatMemento2D, counterOfPlayed,isNoCheat);
				if (isSuperBackedAtLeastOne == false)
				{
					counterOfPlayed++;
					actualIt++;
					for (int i = sizeY - 1; i >= 0; i--)
					{
						for (int j = 0; j < sizeX; j++)
						{
							if (myBoard.get()[j + sizeX*i].IsAlive())
							{
								window.draw(circleShape1);
								circleShape1.setPosition(20 * j, 20 * i);
							}
							else
							{
								window.draw(circleShape2);
								circleShape2.setPosition(20 * j, 20 * i);
							}
						}
					}
				}
				else
				{
					
					for (int i = sizeY - 1; i >= 0; i--)
					{
						for (int j = 0; j < sizeX; j++)
						{
							if (noCheatMemento2D[counterOfPlayed-1][j + sizeX*i] == true)
							{
								window.draw(circleShape1);
								circleShape1.setPosition(20 * j, 20 * i);
							}
							else
							{
								window.draw(circleShape2);
								circleShape2.setPosition(20 * j, 20 * i);
							}
						}
					}
					
					if (counterOfPlayed > 1 && cntToDown)
					{
						counterOfPlayed--;
						actualIt++;
					}
					else if (counterOfPlayed < noCheatMemento2D.size() && !cntToDown)
					{
						counterOfPlayed++;
						actualIt++;
					}
					
				}
				
							
			}
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::KeyPressed)
				{
					
					
				}
				if (event.type == Event::Closed)
				{
					window.close();
				}
				if (event.type == sf::Event::TextEntered)
				{
					if (event.text.unicode == 8) {   // Delete key
						FPSFromTextField = FPSFromTextField.substr(0, FPSFromTextField.size() - 1);
						FPSTEXT.setString(FPSFromTextField);
						tmpFPS = FPSFromTextField;
					}
					else{
						
						FPSFromTextField += event.text.unicode;
						FPSTEXT.setString(FPSFromTextField);
						tmpFPS = FPSFromTextField;
						
					}
				}
			
									
				if (event.type == Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
					//START
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth-buttonWidth) && Mouse::getPosition(window).y >100 && Mouse::getPosition(window).y < (100 + buttonHeight) && !isStarted)
						{
						isStarted = true;
							int it = 0;
							while (it < cordXVec.size())
							{
								numberOfCell = coordinateToNumber(cordXVec[it] + 10, cordYVec[it] + 10, myBoard, sizeX, sizeY);
								myBoard.get()[numberOfCell].setCellAlive();
								if (it == cordXVec.size() - 1)
								{
									for (int i = sizeY - 1; i >= 0; i--)
									{
										for (int j = 0; j < sizeX; j++)
										{
											if (myBoard.get()[j + sizeX*i].IsAlive())
											{
												window.draw(circleShape1);
												circleShape1.setPosition(20 * j, 20 * i);
											}
											else
											{
												window.draw(circleShape2);
												circleShape2.setPosition(20 * j, 20 * i);
											}
										}
									}
								}
								it++;
							}
						}
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >40 && Mouse::getPosition(window).y < (40 + buttonHeight) && !isStarted)
					{
					
						isStarted = true;
						int it = 0;
						//cout << cordXVec2.size() << " ";   --> 0 ??
						while (it < cordXVec2.size())
						{
							//cout << "while2";  nie wchodzi do whilea
							numberOfCell = coordinateToNumber(cordXVec2[it] + 10, cordYVec2[it] + 10, myBoard, sizeX, sizeY);
							myBoard.get()[numberOfCell].setCellAlive();
							if (it == cordXVec2.size() - 1)
							{
								for (int i = sizeY - 1; i >= 0; i--)
								{
									for (int j = 0; j < sizeX; j++)
									{
										if (myBoard.get()[j + sizeX*i].IsAlive())
										{
											window.draw(circleShape1);
											circleShape1.setPosition(20 * j, 20 * i);
										}
										else
										{
											window.draw(circleShape2);
											circleShape2.setPosition(20 * j, 20 * i);
										}
									}
								}
							}
							it++;
						}
					}
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >140 && Mouse::getPosition(window).y < (140 + buttonHeight) && !isStarted)
					{
						
						isStarted = true;
						int it = 0;
						counterOfPattern++;
						while (it < cordXVec3.size())
						{
							//cout << "while3";
							numberOfCell = coordinateToNumber(cordXVec3[it] + 10, cordYVec3[it] + 10, myBoard, sizeX, sizeY);
							myBoard.get()[numberOfCell].setCellAlive();
							if (it == cordXVec3.size() - 1)
							{
								for (int i = sizeY - 1; i >= 0; i--)
								{
									for (int j = 0; j < sizeX; j++)
									{
										if (myBoard.get()[j + sizeX*i].IsAlive())
										{
											window.draw(circleShape1);
											circleShape1.setPosition(20 * j, 20 * i);
										}
										else
										{
											window.draw(circleShape2);
											circleShape2.setPosition(20 * j, 20 * i);
										}
									}
								}
							}
							it++;
						}
					}
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >200 && Mouse::getPosition(window).y < (200 + buttonHeight)  && !isStarted)
					{
						isStarted = true;
						int it = 0;
						while (it < 5)
						{
							cordX[it] = atoi(cordXs[it].c_str());
							cordY[it] = atoi(cordYs[it].c_str());
							numberOfCell = coordinateToNumber(cordX[it], cordY[it], myBoard, sizeX, sizeY);
							myBoard.get()[numberOfCell].setCellAlive();
							if (it == 4)
							{
								for (int i = sizeY - 1; i >= 0; i--)
								{
									for (int j = 0; j < sizeX; j++)
									{
										if (myBoard.get()[j + sizeX*i].IsAlive())
										{
											window.draw(circleShape1);
											circleShape1.setPosition(20 * j, 20 * i);
										}
										else
										{
											window.draw(circleShape2);
											circleShape2.setPosition(20 * j, 20 * i);
										}
									}
								}
							}
							it++;
						}
					}
					// START z klawiatury:
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >600 && Mouse::getPosition(window).y < (600 + buttonHeight) && !isStarted)
					{
						bool selector = true;
						int choice;
						isStarted = true;
						while (selector)
						{
							cout << "Give coordinates to make cell alive: " << endl;
							cout << "coordinateX: ";
							cin >> cordX1;
							cout << endl;
							cout << "coordinateY: ";
							cin >> cordY1;
							numberOfCell = coordinateToNumber(cordX1, cordY1, myBoard, sizeX, sizeY);
							myBoard.get()[numberOfCell].setCellAlive();
							for (int i = sizeY - 1; i >= 0; i--)
							{
								for (int j = 0; j < sizeX; j++)
								{
									if (myBoard.get()[j + sizeX*i].IsAlive())
									{
										window.draw(circleShape1);
										circleShape1.setPosition(20 * j, 20 * i);
									}
									else
									{
										window.draw(circleShape2);
										circleShape2.setPosition(20 * j, 20 * i);
									}
								}
							}
							cout << "DO YOU WANT ALIVE ANOTHER CELL? CHOOSE 1. DO YOU HAVE START GAME? CHOOSE 2" << endl;
							cin >> choice;
							if (choice == 2)
							{
								selector = false;
							}
						}
					}
					//PLAY
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >0 && Mouse::getPosition(window).y < (0 + buttonHeight) && isStarted)
					{
						selectorPlay = true;
						cntToDown = false;

					}
					//STOP
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >700 && Mouse::getPosition(window).y < (700 + buttonHeight) && selectorPlay)
					{
						selectorPlay = false;
						isStopped = true;
						cntToDown = true;
					
					}
					//RUCH
						if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >300 && Mouse::getPosition(window).y < (300 + buttonHeight))
						{

							IsBacked = false;
							IsDidAnyTurn = true;
							/*if (isStopped)
							{
								counterAfterStop++;
								counterOfButt = counterAfterStop;
							}*/
							
							counterOfButt++;
							actualIt++;
							solver(myBoard, neighboursWithLeftTop, neighboursWithRightTop, neighboursWithLeftDown, neighboursWithRightDown, neighboursWithTopRow, neighboursWithDownRow,
								neighboursWithLeftColumn, neighboursWithRightColumn, neighboursMiddle, arrayWithSum, temporaryArray, sizeX, sizeY, memento1D, memento2D, true,noCheatMemento1D, noCheatMemento2D,counterOfPlayed,isNoCheat);
							
								for (int i = sizeY - 1; i >= 0; i--)         //to stad musi pochodzic ten blad
								{
									for (int j = 0; j < sizeX; j++)
									{
										if (memento2D[counterOfButt - 1][j + sizeX*i] == true)     // UWAGA WAZNA ZMIANA Z MY BOARD TO MEMENTO
										{ //myBoard.get()[j + sizeX*i].IsAlive()  wspaniale :) ta zmiana chyba podzialala
											window.draw(circleShape1);
											circleShape1.setPosition(20 * j, 20 * i);
										}
										else
										{
											window.draw(circleShape2);
											circleShape2.setPosition(20 * j, 20 * i);
										}
									}
								}
								
							
							
					}
						//BACK
						if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >400 && Mouse::getPosition(window).y < (400 + buttonHeight) && !selectorPlay && !isStopped)
							
						{
							if (IsDidAnyTurn)
							{
								IsBacked = true;
								IsBackedAtLeastOne = true;

								printBoard(memento2D, circleShape1, circleShape2, sizeX, sizeY, window, counterOfButt);
								if (counterOfButt > 1)
								{
									actualIt++;
									counterOfButt--;
								}
							}
						}
						//SUPERBACK
						if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >800 && Mouse::getPosition(window).y < (800 + buttonHeight) && isStopped)
						{
							isSuperBackedAtLeastOne = true;
							isSuperBacked = true;
							selectorPlay = true;
						}
						
						//FINISH
						if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >500 && Mouse::getPosition(window).y < (500 + buttonHeight))
						{
							for (int i = 0; i < sizeX*sizeY; i++)
							{
								myBoard.get()[i].setCellDead();
							}
							isStarted = false;
							IsBackedAtLeastOne = false;
							IsBacked = false;
							IsDidAnyTurn  =false;
							counterOfButt = 0;
							memento2D.clear();
							noCheatMemento2D.clear();
							selectorPlay = false;
							isStopped = false;
							isSuperBacked = false;
							isNoCheat = false;
							isSuperBackedAtLeastOne = false;
							cntToDown = false;
							counterOfPlayed = 0;
							actualIt = 0;
						}
					}
				}
			}
			actuallFPS = 1 / timeInSecond;
			string actuallFPSString = to_string(actuallFPS);
			Text FPSInfo;
			FPSInfo.setString(actuallFPSString);
			FPSInfo.setFont(font);
			FPSInfo.setCharacterSize(15);
			FPSInfo.setPosition(windowWidth - 100, windowHeight - 50);
			FPSInfo.setFillColor(Color::White);
			window.draw(FPSInfo);
			window.draw(tf);
			window.draw(FPSTEXT);
			window.display();
			endTime = clock();
			clocksTicksTaken = endTime - startTime;
			timeInSecond = clocksTicksTaken / (double)CLOCKS_PER_SEC;
			
	}
	system("pause");
}