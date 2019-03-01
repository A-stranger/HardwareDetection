#include "pch.h"
#include <string> 
#include <iostream>
#include <vector>
#include <fstream> 
#include <regex>
#include <cstring>

using namespace std;


// 描述:execmd函数执行命令，并将结果存储到result字符串数组中
// 参数:cmd表示要执行的命令
// result是执行的结果存储的字符串数组
// 函数执行成功返回1，失败返回0
#pragma warning(disable:4996)
int execmd(char* cmd, char* result) {
	char buffer[128]; //定义缓冲区
	FILE* pipe = _popen(cmd, "r"); //打开管道，并执行命令
	if (!pipe)
		return 0; //返回0表示运行失败
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe)) { //将管道输出到result中
			strcat(result, buffer);
		}
	}
	_pclose(pipe); //关闭管道
	return 1; //返回1表示运行成功
}

void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

void string_replace(string&s1, const string&s2, const string&s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
}

//字符串分割函数
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

string trim(string& a) {

	size_t n = a.find_last_not_of(" \r\n\t");
	if (n != string::npos)
	{
		a.erase(n + 1, a.size() - n);
	}

	n = a.find_first_not_of(" \r\n\t");
	if (n != string::npos)
	{
		a.erase(0, n);
	}
	return a;
}


string newtrim(string &s)
{
	if (s.empty())
	{
		return s;
	}
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

string newtrimline(string &s)
{
	if (s.empty())
	{
		return s;
	}
	s.erase(0, s.find_first_not_of("\r\n"));
	s.erase(s.find_last_not_of("\r\n") + 1);
	return s;
}

int main()
{
	char SystemInstallDate[] = "c:\\windows\\system32\\systeminfo|findstr 初始安装日期";
	char PCserialnumber[] = "wmic bios get serialnumber";
	char MACAddress[] = "ipconfig /all|findstr 物理地址";
	char IPAddress[] = "ipconfig /all|findstr IPv4";
	char MACIPAddress[] = "wmic nicconfig get IPAddress,MACAddress";
	char HDserial[] = "wmic diskdrive get Caption,SerialNumber";

	char SystemInstallDateResult[1024] = "";
	char PCserialnumberResult[1024] = "";
	char MACAddressResult[1024] = "";
	char IPAddressResult[1024] = "";
	char HDserialResult[1024] = "";
	char MACIPAddressResult[1024] = "";

	ofstream fout("计算机配置信息.csv");     //创建一个data.txt的文件

	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;

	if (1 == execmd(SystemInstallDate, SystemInstallDateResult)) {
		string installdate = SystemInstallDateResult;
		
		//cout << installdate << endl;
		cout << "   正在获取系统安装日期 ... ..." << endl;
	
		
		string trimInstallDate = trim(installdate);
		string str = trimInstallDate.substr(14);
		std::vector<std::string> InstallDateResultVector = split(str, ",");
		string finalInstallDateValue = trim(InstallDateResultVector[0]);
		string installDateName = "系统安装日期：";
	//	cout <<"|"<< finalInstallDateValue << "|"<<endl;
		//system("pause"); //暂停以查看结果
		fout << installDateName << "," << finalInstallDateValue << endl;  //将变量的值写入文件
	
	}

	
	if (1 == execmd(PCserialnumber, PCserialnumberResult)) {

		string pcserial = PCserialnumberResult;
		//cout << pcserial << endl;
		string PCSerialString = trim(pcserial);
		vector<string> PCSerialVector = split(PCSerialString, "\r\n");

		string PCSerialValue = newtrim(PCSerialVector[1]);
		string PCSerialKey = "设备序列号：";
		//cout <<"|"<< PCSerialValue << "|"<<endl;
		fout << PCSerialKey << "," << PCSerialValue << endl;  //将变量的值写入文件
		cout << "   正在获取设备序列号 ... ..." << endl;

	}
	
	if (1 == execmd(MACIPAddress, MACIPAddressResult)) {

		string macipaddr = MACIPAddressResult;
		vector<string> MACIPString = split(macipaddr, "\r\n");
		for (int i = 0; i < MACIPString.size();i++) {
			
			string::size_type idx;
			idx = MACIPString[i].find("}");
			if (idx == string::npos) {
			}
				//cout << "null" << endl;
			else {
				//cout << MACIPString[i]<<endl;
				const regex pattern("\\s{2,}");
				const regex maohao(":");
				vector<string> MACandIPVector;
				string temp = regex_replace(MACIPString[i], pattern, "-");
				SplitString(temp, MACandIPVector, "-");

				string ipString = MACandIPVector[0];
				string macString = MACandIPVector[1];
				string MACValue = regex_replace(macString, maohao, "-");
				int beginIP = ipString.find_first_of(",");
				string IPValue = ipString.substr(2,beginIP-3);
			//	cout << IPValue << "|" << MACValue << "|"<<endl;
				fout << "MAC地址："+MACValue << "," << "IP地址："+IPValue << endl;  //将变量的值写入文件
			}

		}
				cout << "   正在获取网卡MAC地址和IP地址 ... ..." << endl;

	}
	
	if (1 == execmd(HDserial, HDserialResult)) {
		string hdserial = HDserialResult;
		string hdvar, hdvar1, hdvar2;
		vector<string> HDVector;
		SplitString(newtrimline (hdserial), HDVector, "\r\n");
		//	cout << hdserial<<endl;


		for (int k = 1; k < HDVector.size();k++) {
			vector<string> HDKeyValue;
			string tempHDSerialstring = newtrim(HDVector[k]);
			int beginHDSerial = tempHDSerialstring.find_last_of(" ");
			
			string HDserialValue = tempHDSerialstring.substr(beginHDSerial+1, tempHDSerialstring.size());
			string HDserialKey = tempHDSerialstring.substr(0, beginHDSerial);
			
			char str[2];
			itoa(k,str,10);
	
			
			//cout << "|" << HDserialKey <<"|"<< HDserialValue << "|" << endl;
			fout << "硬盘"<< str<< "("<<newtrim(HDserialKey)<<")序列号为：" << "," << HDserialValue << endl;  //将变量的值写入文件

			//fout << "硬盘"<< str<<"序列号为："+HDserialKey << "," << HDserialValue << endl;  //将变量的值写入文件

		/*	| Lenovo SSD SL700 M.2 128G  A5A6076C0C8D00000114  |
				|TOSHIBA MQ01ACF050         44L4C9RYT |*/
		
		}
			cout << "   正在获取硬盘序列号 ... ..." << endl;

	}


	fout.close();                  //关闭文件
	cout << "   完成。 " << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << "   ==========================================================" << endl;
	cout << "   =                                                        =" << endl;
	cout << "   =                     程序执行完毕 ！！！                =" << endl;
	cout << "   =                                                        =" << endl;
	cout << "   =     文件  计算机配置信息.csv  已输出到程序所在目录。   =" << endl;
	cout << "   =                                                        =" << endl;
	cout << "   ==========================================================" << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	system("start 计算机配置信息.csv");
	system("pause"); //暂停以查看结果


	return 0;
}