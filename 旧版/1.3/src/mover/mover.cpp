#include <stdio.h>
#include <windows.h>//system
#include <iostream>
#include <conio.h>//puts
#include <string> 
#include <io.h>
#include <stdlib.h>//atoi
using namespace std;
#define fn 100
#define fl 255
#define jpgn 1000
#define FiPath "E:\\Temporary\\1Check\\2Check\\(Batch)"
#define ChPath "E:\\Temporary\\新建文件夹"

int radefile(char finame[fn][fl])
{
	system("dir /b "FiPath">>buffile.txt");//将目录导出到buf.txt
	FILE *fp = fopen("buffile.txt","rb");
	
	int i=0;
    while (!feof(fp))
    {		
        fgets(finame[i],fl,fp);
		i++;
    }
	
    fclose(fp);
	
	//	system("del /Q buffile.txt");  //删除目录下buf.txt文件
	
	return i-1;
}

int str2cha(string str,char cha[fl])
{
	int suc=0;
	int i=0;
	//	cout<<str.length()<<endl;
	if(str.length()!=0)//数组不为空
	{
		for(;i<str.length();i++)
			cha[i] = str[i];
		//		cha[i] = '\n';//每行包括一个换行符 终止符
		//		cout<<cha<<endl;//浏览转换结果
		suc=1;
		cha[i] = '\0';
	}
	else
	{
		//		cha[0] = '\n';
		suc=2;
	}
	
	for(;i>str.length()-3;i--)
	{
		if (cha[i]==13 || cha[i]==10 ||cha[i]==33)
			cha[i]='\0';
	}
	//cout<<cha<<endl;
	
	return suc;
}

void stacha(char staedcha[fl])
{
	string staedstr=staedcha;
	str2cha(staedstr,staedcha);
}

void jiadizhi(char beijia[fl],int ifyuan)
{
	string strbeijia=beijia;
	if (ifyuan==1)
		strbeijia.insert(0,FiPath"\\");
	else if (ifyuan==0)
		strbeijia.insert(0,ChPath"\\");
	str2cha(strbeijia,beijia);
}

void tempfile(char tempna[fl],int ifyuan)
{
	string tempstr=tempna;
	if (ifyuan==1)
		tempstr.insert(0,FiPath"\\[temp]");
	else if (ifyuan==0)
		tempstr.insert(0,ChPath"\\[temp]");
	str2cha(tempstr,tempna);
}


int	radedata(char data[jpgn][fl],char finame[fl],int ifyuan,char finameNei[jpgn][fl])
{
	
	jiadizhi(finame,ifyuan);
	//	cout<<finame<<endl;
	
	char tempna[fl]="\0";//临时文件夹名
	tempfile(tempna,ifyuan);
	
	//	if(!_access(finame,0))//如果文件存在:
	//		cout<<" 存在."<<endl;
	
	rename(finame, tempna);
	//	if (rename(finame, tempna) == 0)//转换为临时文件名
	//		cout<<"Renamed succed."<<endl;
	
	/*
	string finamestr=finame;
	
	  
	*/
	//system(cmdcha);//将目录导出到buf.txt
	if (ifyuan==1)
	{
		system("dir /-c "FiPath"\\[temp]>>bufdatac.txt");
		system("dir /b "FiPath"\\[temp]>>bufdatab.txt");
	}
	else if (ifyuan==0)
		system("dir /-c "ChPath"\\[temp]>>bufdatac.txt");
	
	//	rename(tempna, finame);
	//	if (rename(tempna, finame) == 0)//转换为原文件名
	//		cout<<"Renamed succed."<<endl;
	;
	
	FILE *fp = fopen("bufdatac.txt","rb");//文件大小数据
	
	int i=0;
    while (!feof(fp))
    {		
        fgets(data[i],fl,fp);
		stacha(data[i]);
		//	cout<<data[i]<<endl;
		i++;
    }
	
    fclose(fp);
	system("del /Q bufdatac.txt");  //删除目录下buf.txt文件
	
	if (ifyuan==1)
	{
		fp = fopen("bufdatab.txt","rb");//文件名
		
		int j=0;
		while (!feof(fp))
		{		
			fgets(finameNei[j],fl,fp);
			stacha(finameNei[j]);
			//		cout<<finameNei[j]<<endl;
			j++;
		}
		
		fclose(fp);
		system("del /Q bufdatab.txt");  //删除目录下buf.txt文件	
	}
	
	
	return i-1;
}

int check(char bechecked[fl])
{
	string checked=bechecked;
	if (checked.rfind(".")==-1)
		return 0;
	else
		return 1;
}

void viewcha(char cha[fn][fl],int chacnt)
{
	for(int i=0;i<chacnt;i++)
	{
		puts(cha[i]); //显示文件名
	}
	cout<<endl;
}


void radesize(char data[jpgn][fl],float rnum[jpgn],int datasize)
{
	char buf[jpgn][fl];
	
	//	viewcha(data,datasize);
	
	for(int i=0;i<datasize;i++)
	{
		sscanf(data[i],"%*s%*s%s",buf[i]);
		//		printf("%s\n",buf[i]);
	}
	
	float num[jpgn];
	int ri=0;
	float sizecnt=0;
	
	
	for(i=0;i<datasize;i++)
	{
		num[i]=atoi(buf[i]);
		//		cout<<num[i]<<endl;
		if(num[i]==0)
			continue;
		if(sizecnt!=num[i])
		{
			sizecnt+=num[i];
			rnum[ri]=num[i];
			ri++;
		}
	}
	/*	
	for(i=0;i<datasize-9;i++)
	{
	cout<<rnum[i]<<endl;
	//		printf("%s\n",buf[i]);
	}
	*/	
}

void displayProgress(int progress)
{
	progress=progress*30/100;
	int k = 0;
	for (k=0; k<100; k++)
		putchar('\r');//将当前行全部清空，用以显示最新的进度条状态
	int j = 0;
	for (j=0; j<progress; j++)
		putchar('+');//打印进度条上已经完成的部分，用‘+’表示
	for (j=1; j<=30-progress; j++)
		putchar('-');//打印进度条上还有多少没有完成的
	fprintf(stdout, "  %3d%%",progress*10/3);
	fflush(stdout);
}

/*
void tojpg(string toedstring)
{
int idx=0;
idx=toedstring.rfind(".png");
if (idx!=-1)
toedstring.replace(idx,4,".jpg");

  idx=0;
  idx=toedstring.rfind(".jpeg");
  if (idx!=-1)
		toedstring.replace(idx,5,".jpg");
		}
*/

int cut(char finameNei[jpgn][fl],float rnum[2][jpgn],int datasize)
{
	string fipath;
	string chpath;
	string cmd;
	string empty;
	char cmdcha[fl];
	char oldname[fl];
	char newname[fl];
	//char fipathcha[fl];
	//char chpathcha[fl];
	
	
	int idx=0;
	int process=0;
	int changcnt=0;
	for (int i=0;i<datasize;i++)//datasize
	{
		process=0;
		if ((rnum[0][i]-rnum[1][i]) > (rnum[0][i]/33))//转换后的文件小3%以上
		{
			fipath=FiPath;
			fipath.append("\\[temp]\\");
			fipath.append(finameNei[i]);
			//	cout<<fipath<<endl;
			
			chpath=ChPath;
			chpath.append("\\[temp]\\");
			chpath.append(finameNei[i]);
			//		tojpg(chpath);
			idx=0;
			idx=chpath.rfind(".png");
			if (idx!=-1)
				chpath.replace(idx,4,".jpg");
			
			idx=0;
			idx=chpath.rfind(".jpeg");
			if (idx!=-1)
				chpath.replace(idx,5,".jpg");
			
			//cout<<chpath<<endl;
			
			/*str2cha(fipath,fipathcha);
			if(_access(fipathcha,0))//如果文件存在:
			cout<<" 不存在."<<endl;
			str2cha(chpath,chpathcha);
			if(!_access(chpathcha,0))//如果文件存在:
			cout<<" 存在."<<endl;
			*/
			
			cmd=empty;
			cmd.append("del /Q ");
			cmd.append("\"");
			cmd.append(fipath);
			cmd.append("\"");
			//cout<<cmd<<endl;
			str2cha(cmd,cmdcha);
			stacha(cmdcha);
			system(cmdcha);//删除源文件
			
						   /*
						   cmd=empty;
						   cmd.append("copy ");
						   cmd.append(chpath);
						   cmd.append(" ");
						   cmd.append(fipath);
						   //	cout<<cmd<<endl;
						   str2cha(cmd,cmdcha);
						   stacha(cmdcha);			
						   system(cmdcha);//复制文件文件
			*/
			
			//		tojpg(fipath);
			idx=0;
			idx=fipath.rfind(".png");
			if (idx!=-1)
				fipath.replace(idx,4,".jpg");
			
			idx=0;
			idx=fipath.rfind(".jpeg");
			if (idx!=-1)
				fipath.replace(idx,5,".jpg");
			
				/*	chpath.insert(0,"\"");//处理空格
				chpath.insert(chpath.length(),"\"");
				fipath.insert(0,"\"");
			fipath.insert(fipath.length(),"\"");*/
			
			//cout<<chpath<<endl<<fipath<<endl;
			rename(chpath.c_str(), fipath.c_str());
			changcnt++;
		}
		displayProgress(i*100/datasize);
	}
	displayProgress(100);
	cout<<endl;
	return changcnt;
}

void rechan(char finame[fl],int ifyuan)
{
	jiadizhi(finame,ifyuan);
	char tempna[fl]="\0";//临时文件夹名
	tempfile(tempna,ifyuan);
	rename(tempna, finame);
}


void changcomb(char finame[fl])
{
	char finametemp[fl];
	strcpy(finametemp,finame);
	rechan(finametemp,1);//变回原文件名
	strcpy(finametemp,finame);
	rechan(finametemp,0);
	
}

main()
{
	char finame[fn][fl];
	char finametemp[fl];
	char finameNei[jpgn][fl];
	char data[jpgn][fl];
	
	int ficnt=radefile(finame);
	
	
	//jiadizhi(finame[0],1);
	
	//cout<<finame[0]<<endl;
	
	//	viewcha(finame,ficnt);
	
	float rnum[2][jpgn];
	int datasize=0;
	int changcnt=0;
	for(int i=0;i<ficnt;i++)
	{
		stacha(finame[i]);
		strcpy(finametemp,finame[i]);//保证文件名信息不变
		
		cout<<"Start work in the file"<<endl;
		
		jiadizhi(finametemp,1);
		if(_access(finametemp,0))//如果文件不存在:
			cout<<finametemp<<endl<<"do not exit."<<endl<<endl;
		else
		{
			cout<<finame[i]<<endl;
			
			strcpy(finametemp,finame[i]);//保证文件名信息不变
			datasize=radedata(data,finametemp,1,finameNei);//9行文本信息
			if(!check(finameNei[0]))
			{
				changcomb(finame[i]);
				continue;
			}
			//	cout<<datasize<<endl;
			radesize(data,rnum[0],datasize);
			//	viewcha(finameNei,38);
			//	cout<<endl;
			
			strcpy(finametemp,finame[i]);
			radedata(data,finametemp,0,finameNei);
			//	cout<<datasize<<endl;
			radesize(data,rnum[1],datasize);
			
			changcnt=cut(finameNei,rnum,datasize-9);
			changcomb(finame[i]);
			
			cout<<changcnt<<" pictures have been removed"<<endl<<endl;
		}
	}
	
	cout<<"Press any key to exit"<<endl;
	
	char tuichu=getch();
	system("del /Q buffile.txt");  //删除目录下buf.txt文件
	
}