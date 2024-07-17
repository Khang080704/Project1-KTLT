#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <string>
#include <fstream>
#include <locale>
#include <Windows.h>
#include <codecvt>
#include <sstream>
#include <vector>
#include <direct.h>
using namespace std;
const int ATTRIBUTES = 9;

struct SinhVien {
    wstring MSSV;
    wstring name;
    wstring Khoa;
    int Year;
    wstring birthday;
    wstring image;
    wstring describe;
    vector <wstring> habbit;
};
enum infor {
    MSSV = 1,
    name = 2,
    Khoa = 3,
    Year = 4,
    birthday = 5,
    image = 6,
    describe = 7,
    habbit = 8
};
void generateCSS(wstring pathFile);
static void showConfiguration();
void generateImageFolder(wstring path);
void processFile(wstring& fileName);
void createImage(wstring path, SinhVien* t, int n);
void changeInfo(SinhVien& t, int num);
void generateHTML(wstring pathFile, SinhVien* t, int n, bool* configuration, int configSize);
bool checkAllFalse(bool* array, int);

int main()
{

    //set up doc ghi file unicode
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    locale loc(locale(), new codecvt_utf8<wchar_t>);
    wifstream fi;
    wofstream fo;

    //Doc file unicode input (file input nam ke ham main)
    fi.open(L"input.txt");
    fi.imbue(loc);
    fo.open(L"output.txt");

    //input path
    wstring path = L"";
    wcout << "Nhap vao duong dan: ";
    getline(wcin, path);
    showConfiguration();
    wcout << L"Nhập vào các lựa chọn( cách nhau bởi khoảng trắng), nhấn enter để bỏ qua( phát sinh với đầy đủ cấu hình): ";
    vector<int> chooses;
    wstring user = L"";
    
    getline(wcin, user, L'\n');
    wstringstream split(user);
    wstring number;
    while (split >> number) {
        chooses.push_back(stoi(number));
    }
        
    bool* configChoose = new bool[ATTRIBUTES] {false};
    for (int i : chooses) {
        configChoose[i] = true;
    }
    
    wstring s;
    SinhVien* t = new SinhVien[100];
    wstring attribute;
    int count = 0;
    string buffer;
    while (!fi.eof())
    {
        getline(fi, s, L'\n');
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == '"') {
                s[i] = ' ';
            }
        }
        wstringstream ss(s);
        getline(ss, attribute, L',');
        t[count].MSSV = attribute; //MSSV
        getline(ss, attribute, L',');
        t[count].name = attribute; //Ho ten
        getline(ss, attribute, L',');
        t[count].Khoa = attribute; // Khoa
        getline(ss, attribute, L',');
        t[count].Year = stoi(attribute); //Nam vao khoa
        getline(ss, attribute, L',');
        t[count].birthday = attribute; // Ngay sinh
        getline(ss, attribute, L',');
        t[count].image = attribute; //Link toi file anh
        getline(ss, attribute, L',');
        t[count].describe = attribute; // Mo ta ban than
        /* getline(ss, attribute, L',');
         t[count].habbit = attribute;*/
        while (getline(ss, attribute, L';')) {
            t[count].habbit.push_back(attribute); // Cac so thich
        }
        count++;
    }
    generateCSS(path);
    generateHTML(path, t, count, configChoose,ATTRIBUTES);
    generateImageFolder(path);
    createImage(path, t, count);

    fi.close();
    fo.close();
    delete[] t;

    return 0;
}
void generateHTML(wstring pathFile, SinhVien* t, int n, bool* configuration, int configSize) {
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    locale loc(locale(), new codecvt_utf8<wchar_t>);

    for (int i = 0; i < n; i++) {
        wstring input = pathFile + L"/" + t[i].MSSV + L".html";
        wofstream writter(input, ios::out);
        writter.imbue(loc);
        if (checkAllFalse(configuration, configSize)) {
            //do nothing
        }
        else {
            for (int j = 1; j < configSize; j++) {
                if (configuration[j] == false) {
                    changeInfo(t[i], j);
                }
            }
        }

        writter << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">" << endl
            << "<html xmlns = \"http://www.w3.org/1999/xhtml\">" << endl
            << "<head>" << endl
            << "<meta http - equiv = \"Content-Type\" content = \"text/html; charset=utf-8\" / >" << endl
            << "<link rel = \"stylesheet\" type = \"text/css\" href = \"./personal.css\" />" << endl
            << L"<title>HCMUS -" << t[i].name << "</title>" << endl
            << "</head>" << endl
            << "<body>" << endl
            << "<div class = \"Layout_container\">" << endl

            << "<!--Begin Layout MainContents Region-->" << endl
            << "<div class = \"Layout_MainContents\">" << endl

            << "<div class = \"Personal_Main_Information\">" << endl
            << "<div class = \"Personal_Location\">" << endl
            << "<div class = \"Personal_FullName\">" << t[i].name << "-" << t[i].MSSV << "</div>" << endl
            << "<div class = \"Personal_Department\">" << t[i].Khoa << "</div>" << endl
            << "</br>" << endl
            << "<div class = \"Personal_Phone\">" << endl
            << "</div>" << endl
            << "<br / >" << endl
            << "<br / >" << endl
            << "</div>" << endl
            << "<div class = \"Personal_HinhcanhanKhung\">" << endl;
        if (configuration[infor::image] == false && checkAllFalse(configuration,configSize)) {
            writter << "<img src = \"./Image/" << t[i].MSSV << ".jpg \" class = \"Personal_Hinhcanhan\"/ >" << endl;

        }
           writter << "</div>" << endl
            << "</div>" << endl
            //<!--End Below Banner Region-->
            //<!--Begin MainContents Region-->
            << "<div class = \"MainContain\">" << endl

            //<!--Begin Top Region-->
            << "<div class = \"MainContain_Top\">" << endl

            << L"<div class = \"InfoGroup\">Thông tin cá nhân</div>" << endl
            << "<div>" << endl
            << "<ul class = \"TextInList\">" << endl
            << L"<li>Họ và tên :" << t[i].name << "</li>" << endl
            << "<li>MSSV :" << t[i].MSSV << "</li >" << endl
            << L"<li>Khoa:" << t[i].Khoa << "</li>" << endl
            << "<li>Ngày sinh :" << t[i].birthday << "</li>" << endl
            << "</ul>" << endl
            << "</div>" << endl
            << L"<div class = \"InfoGroup\">Sở thích</div>" << endl
            << "<div>" << endl
            << "<ul class = \"TextInList\">" << endl;
        for (int j = 0; j < t[i].habbit.size(); j++) {
            writter << L"<li>" << t[i].habbit[j] << "</li>" << endl;
        }

        writter << "</ul>" << endl
            << "</div>" << endl
            << L"<div class = \"InfoGroup\">Mô tả</div>" << endl
            << "<div class = \"Description\">" << endl
            << t[i].describe << endl
            << "</div>" << endl
            << "</div>" << endl
            << "</div>" << endl
            << "</div>" << endl

            // <!--Begin Layout Footer-->
            << "<div class = \"Layout_Footer\">" << endl
            << "<div class = \"divCopyright\">" << endl
            << "<br / >" << endl
            << "<br / >" << endl
            << "@2024 </br >" << endl
            << L"Đồ án giữa kỳ</br>" << endl
            << L"Kỹ thuật lập trình</br>" << endl
            << "TH2024</br>" << endl
            << L"22120152 - Phạm Gia Khang</br>" << endl
            << "</div>" << endl
            << "</div>" << endl
            //<!--End Layout Footer-->
            << "</div>" << endl
            << "</body>" << endl
            << "</html>" << endl;

        writter.close();
    }
}
void generateCSS(wstring pathFile) {
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    locale loc(locale(), new codecvt_utf8<wchar_t>);

    wifstream reader("Personal.txt", ios::in);
    wstring outputCSS = pathFile + L"/" + L"Personal.css";
    wofstream writter(outputCSS, ios::out);
    while (!reader.eof()) {
        wstring buffer;
        getline(reader, buffer);
        writter << buffer << endl;
    }
    reader.close();
    writter.close();
}
static void showConfiguration() {
    wcout << L"LỰA CHỌN THÔNG TIN XUẤT HIỆN TRÊN WEBSITE: " << endl;
    wcout << endl;
    wcout << L"1. Mã số sinh viên" << endl;
    wcout << L"2. Họ tên" << endl;
    wcout << L"3. Khoa" << endl;
    wcout << L"4. Năm vào khoa" << endl;
    wcout << L"5. Ngày sinh" << endl;
    wcout << L"6. Hình ảnh cá nhân" << endl;
    wcout << L"7. Mô tả bản thân" << endl;
    wcout << L"8. Sở thích" << endl;
}
//Tao folder hinh anh
void generateImageFolder(wstring path) {
    wstring imagepath = path + L"/Image";
    CreateDirectory(imagepath.c_str(), NULL);
}
//Xu li khoang trang o dau ten file
void processFile(wstring& fileName) {
    wstringstream iss(fileName);
    iss >> ws >> fileName;
}
//Tao image dat vao duong dan dua tren image da co
void createImage(wstring path, SinhVien* t, int n) {
    const int BUFFERSIZE = 1024;
    for (int i = 0; i < n; i++) {
        processFile(t[i].image);
        wifstream reader(t[i].image, ios::binary);
        wchar_t byte[BUFFERSIZE];

        //Ghi file jpg vao file tam temp.bin
        wofstream tempFile(L"temp.bin", ios::binary);
        while (reader.read(byte, BUFFERSIZE)) {
            tempFile.write(reinterpret_cast<const wchar_t*>(&byte), reader.gcount());
        }


        //Load data tu file temp.bin de ghi vao file jpg moi
        wstring pathInput = path + L"/Image/" + t[i].MSSV + L".jpg";
        wifstream readTempFile(L"temp.bin", ios::binary);
        wofstream writter(pathInput, ios::binary);
        while (readTempFile.read(byte, BUFFERSIZE)) {
            writter.write(reinterpret_cast<const wchar_t*>(&byte), readTempFile.gcount());

        }
        tempFile.close();
        reader.close();
        readTempFile.close();
        writter.close();
    }
}
void changeInfo(SinhVien& t, int num) {
    switch (num)
    {
    case infor::MSSV: {
        t.MSSV = L"";
        break;
    }
    case infor::name: {
        t.name = L"";
        break;
    }
    case infor::Khoa: {
        t.Khoa = L"";
        break;
    }
    case infor::Year: {
        break;
    }
    case infor::birthday: {
        t.birthday = L"";
        break;
    }
    case infor::image: {
        t.image = L"";
        break;
    }
    case infor::describe: {
        t.describe = L"";
        break;
    }
    case infor::habbit: {
        t.habbit.clear();
        break;
    }
    default:
        break;
    }
}
bool checkAllFalse(bool* array, int n) {
    for (int i = 0; i < n; i++) {
        if (array[i] == true) {
            return false;
        }
    }
    return true;
}


//
//#include<iostream>
//#include<fstream>
//#include<string>
//#include<sstream>
//#include<vector>
//
//using namespace std;
//struct sv {
//	string mssv;
//	string hoten;
//	string khoa;
//	string nienKhoa;
//	vector<string> soThich;
//};
//void processFile(wstring& fileName) {
//    wstringstream iss(fileName);
//    iss >> ws >> fileName;
//}
//int main()
//{
//	wstring output = L"   Doraemon.jpg";
//	processFile(output);
//	wifstream reader(output, ios::binary);
//	wofstream writter("test.bin",ios::binary);
//	wchar_t byte[1024];
//	int count = 0;
//	while (reader.read(byte,1024)) {
//		writter.write(reinterpret_cast<const wchar_t*>(&byte), reader.gcount());
//		count++;
//	}
//	count = 0;
//	wifstream binary("test.bin", ios::binary);
//	wofstream jpgFille("F:/Image/test.jpg", ios::binary);
//	while (binary.read(byte, 1024)) {
//		jpgFille.write(byte, binary.gcount());
//		count++;
//	}
//
//	jpgFille.close();
//	binary.close();
//	reader.close();
//	writter.close();
//
//
//	return 0;
//}