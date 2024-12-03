#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cout << "Use:\n"	 
			<< "	text2table source destination" << std::endl;
		std::system("pause");

		return 1;
	}


	std::ifstream inFile(argv[1]);

	if (!inFile) {
		std::cout << "Can't open file " << argv[1] << std::endl;
		std::system("pause");

		return 2;
	}


	//1. Пройти по файлу и подсчитать ширины столбцов
	int lenwords[200] = { 0 };
	int wordlen = 0;
	int wordnum = 0;
	int ch;

	while ((ch = inFile.get()) != EOF) {
		if (ch == ' ' || ch == '\t') {
			if (wordlen != 0) {
				if (lenwords[wordnum] < wordlen) {
					lenwords[wordnum] = wordlen;
				}

				wordlen = 0;
				++wordnum;
			}
		}
		else if (ch == '\n') {
			if (lenwords[wordnum] < wordlen) {
				lenwords[wordnum] = wordlen;
			}

			wordlen = 0;
			wordnum = 0;
		}
		else {
			++wordlen;
		}
	}

	if (wordlen != 0) {
		if (lenwords[wordnum] < wordlen) {
			lenwords[wordnum] = wordlen;
		}
	}
#if 0
	for (int i = 0; i < 200; ++i) {
		std::cout << lenwords[i] << " ";
	}
#endif

	//2. Пройти по файлу и добавлять к словам так, чтобы ширина слова равна ширине столбца
	inFile.clear();
	inFile.seekg(0);

	std::ofstream outFile(argv[2]);
	if (!outFile.is_open()) {
		std::cout << "Can't write in the file " << argv[2] << "\n";
		inFile.close();
		return 3;
	}

	wordlen = 0;
	wordnum = 0;
	int charCounter = 0;
	while ((ch = inFile.get()) != EOF) {
		if (ch == ' ' || ch == '\t') {
			if (wordlen != 0) {
				++wordnum;
				wordlen = 0;
			}
		}
		else if (ch == '\n') {
			outFile.put('\n');
			wordnum = 0;
			wordlen = 0;
			charCounter = 0;
		}
		else {
			if (wordlen == 0 && wordnum > 0) { // Вставить пробелы перед словом
				while (charCounter < lenwords[wordnum]) {
					outFile.put(' ');
					++charCounter;
				}
				charCounter = 0;
			}
			outFile.put(ch);
			++wordlen;
			++charCounter;
		}
	}
	inFile.close();
	outFile.close();
	//std::system("pause");	

	return 0;
}