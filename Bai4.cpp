#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

#define TABLE_SIZE 15013

using namespace std;

struct WordDef {
    string word;
    string type;
    string meaning;

    void WriteToFile(ofstream& outFile) const
    {
        size_t wordSize = word.size();
        size_t typeSize = type.size();
        size_t meaningSize = meaning.size();

        outFile.write(reinterpret_cast<const char*>(&wordSize), sizeof(wordSize));
        outFile.write(word.c_str(), wordSize);

        outFile.write(reinterpret_cast<const char*>(&typeSize), sizeof(typeSize));
        outFile.write(type.c_str(), typeSize);

        outFile.write(reinterpret_cast<const char*>(&meaningSize), sizeof(meaningSize));
        outFile.write(meaning.c_str(), meaningSize);
    }

    void ReadFromFile(ifstream& inFile)
    {
        size_t wordSize, typeSize, meaningSize;

        inFile.read(reinterpret_cast<char*>(&wordSize), sizeof(wordSize));
        word.resize(wordSize);
        inFile.read(&word[0], wordSize);

        inFile.read(reinterpret_cast<char*>(&typeSize), sizeof(typeSize));
        type.resize(typeSize);
        inFile.read(&type[0], typeSize);

        inFile.read(reinterpret_cast<char*>(&meaningSize), sizeof(meaningSize));
        meaning.resize(meaningSize);
        inFile.read(&meaning[0], meaningSize);
    }
};

vector<WordDef> hashTable[TABLE_SIZE];

int HashFunction(const string& key)
{
    int hashVal = 0;
    for (char ch : key)
        hashVal = (hashVal * 31 + ch) % TABLE_SIZE;

    return hashVal;
}

void NormalizeString(string& str, bool capitalize = false)
{
    string result;
    bool inSpace = false;

    for (char ch : str)
    {
        if (isspace(ch))
        {
            if (!inSpace)
            {
                result += ' ';
                inSpace = true;
            }
        }
        else
        {
            result += ch;
            inSpace = false;
        }
    }

    if (!result.empty() && result.front() == ' ')
        result.erase(result.begin());
    if (!result.empty() && result.back() == ' ')
        result.pop_back();

    transform(result.begin(), result.end(), result.begin(), ::tolower);

    if (capitalize && !result.empty())
        result[0] = toupper(result[0]);

    str = result;
}

bool AddWord(WordDef& newWord)
{
    int index = HashFunction(newWord.word);

    for (const WordDef& word : hashTable[index])
    {
        if (word.word == newWord.word)
        {
            cout << "Tu '" << newWord.word << "' da duoc dinh nghia trong tu dien." << endl;
            return false;
        }
    }

    cout << "    Nhap loai tu: ";
    getline(cin, newWord.type);
    cout << "    Nhap nghia tieng Viet: ";
    getline(cin, newWord.meaning);
    NormalizeString(newWord.type);
    NormalizeString(newWord.meaning, true);

    hashTable[index].push_back(newWord);
    cout << "Them dinh nghia thanh cong!" << endl;
    return true;
}

void SearchWord(const string& searchWord)
{
    int index = HashFunction(searchWord);

    for (const WordDef& word : hashTable[index])
    {
        if (word.word == searchWord)
        {
            cout << "Ket qua: " << word.meaning << " (" << word.type << ")" << endl;
            return;
        }
    }

    cout << "Tu '" << searchWord << "' chua duoc cap nhat vao tu dien." << endl;
}

bool RemoveWord(const string& wordToRemove)
{
    int index = HashFunction(wordToRemove);

    auto& bucket = hashTable[index];
    auto it = remove_if(bucket.begin(), bucket.end(), [&wordToRemove](const WordDef& word) { return word.word == wordToRemove; });

    if (it == bucket.end())
    {
        cout << "Khong tim thay tu de xoa!" << endl;
        return false;
    }

    bucket.erase(it, bucket.end());
    cout << "Da xoa dinh nghia '" << wordToRemove << "' khoi tu dien." << endl;
    return true;
}

bool UpdateWord(const string& wordToUpdate)
{
    int index = HashFunction(wordToUpdate);

    for (WordDef& word : hashTable[index])
    {
        if (word.word == wordToUpdate)
        {
            cout << "Nhap thong tin moi cho tu: " << word.word << endl;
            cout << "\tLoai tu moi: ";
            getline(cin, word.type);
            cout << "\tNghia moi: ";
            getline(cin, word.meaning);

            NormalizeString(word.type);
            NormalizeString(word.meaning, true);
            cout << "Cap nhat tu dien thanh cong!" << endl;
            return true;
        }
    }

    cout << "Khong tim thay tu!" << endl;
    return false;
}

// Ghi bảng băm vào file
void SaveToFile()
{
    ofstream fo("dictionary.bin", ios::binary | ios::trunc);

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        for (const WordDef& word : hashTable[i])
            word.WriteToFile(fo);
    }

    fo.close();
}

// Tải từ điển từ file vào bảng băm
void LoadFromFile()
{
    ifstream fi("dictionary.bin", ios::binary);
    if (!fi)
    {
        ofstream fo("dictionary.bin");
        if (fo.is_open())
        {
            fo.close();
        }else
            cout << "Loi: Khong the tao file dictionary.bin";

        return;
    }

    while (!fi.eof())
    {
        WordDef word;
        word.ReadFromFile(fi);
        if (fi.eof()) break;

        int index = HashFunction(word.word);
        hashTable[index].push_back(word);
    }

    fi.close();
}

void clrscr()
{
    #ifdef _WIN32
        system("CLS"); // cai nay cho window
    #else
        system("clear"); // Cai nay cho linux
    #endif
}

void Menu()
{
    while (true)
    {
        clrscr();
        cout << "\n--------- TU DIEN ANH-VIET ---------" << endl;
        cout << "1. Tra tu tieng Anh" << endl;
        cout << "2. Them tu moi vao tu dien" << endl;
        cout << "3. Xoa tu co san khoi tu dien" << endl;
        cout << "4. Cap nhat tu co san khoi tu dien" << endl;
        cout << "5. Luu va thoat" << endl;
        cout << "------------------------------------" << endl;

        string input;
        cout << "Nhap lua chon: ";
        cin >> input;
        NormalizeString(input);
        int choice;
        if (input.size() == 1)
        {
            choice = static_cast<int>(input[0]) - 48;
            if (choice < 1 || choice > 5)
                input = "khong";
        }
        else
            input = "khong";

        if (input != "khong")
        {
            cin.ignore();
            string word;
            WordDef newWord;
            cout << "------------------------------------" << endl;
            switch (choice) {
                case 1:
                    cout << "Nhap tu can tra: ";
                    getline(cin, word);
                    NormalizeString(word);
                    if (word == "")
                        cout << "Loi: khong co tu nao duoc nhap vao!" << endl;
                    else
                        SearchWord(word);
                    break;
                case 2:
                    cout << "Nhap tieng Anh: ";
                    getline(cin, newWord.word);
                    NormalizeString(newWord.word);
                    if (newWord.word == "")
                        cout << "Loi: khong co tu nao duoc nhap vao!" << endl;
                    else
                        AddWord(newWord);
                    break;
                case 3:
                    cout << "Nhap tu can xoa: ";
                    getline(cin, word);
                    NormalizeString(word);
                    if (word == "")
                        cout << "Loi: khong co tu nao duoc nhap vao!" << endl;
                    else
                        RemoveWord(word);
                    break;
                case 4:
                    cout << "Nhap tu can cap nhat: ";
                    getline(cin, word);
                    NormalizeString(word);
                    if (word == "")
                        cout << "Loi: khong co tu nao duoc nhap vao!" << endl;
                    else
                        UpdateWord(word);
                    break;
                case 5:
                    SaveToFile();
                    cout << "Da luu tu dien va thoat chuong trinh." << endl;
                    return;
            }
        }
        else
        {
            cout << "Lua chon khong hop le. Vui long chon lai!" << endl;
        }
        cout << "Nhan phim bat ki de tiep tuc...";
        cin.get();
    }
}

int main()
{
    LoadFromFile();
    Menu();
    return 0;
}
