#include <iostream>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <queue>
using namespace std;

class dict{
string dict_key;
string dict_value;
public:
dict(string key,string value){
dict_key=key;
dict_value=value;
}
string getkey();
string getvalue();
void show();
};
string dict::getkey(){
return dict_key;
}
string dict::getvalue(){
return dict_value;
}
void dict::show(){
cout<<"debug key:"<<dict_key<<endl;
cout<<"debug value:"<<dict_value<<endl;

}


string getcmd(string cmd){
	FILE * fp;
	char buffer[8000000];
	fp=popen(cmd.data(),"r");
	fgets(buffer,sizeof(buffer),fp);
	string result=buffer;
	return result;

}
int getstr(string a,string searchstr,int start){
	int index=a.find(searchstr,start);
	if(index==-1){
		return -1;
	}
	int begin_a=index+searchstr.length();
	return begin_a;
}

void renren(string ip,string port,string passwd,string uploadprogram,string uploaddir,string downloaddir,string panf){
string API_URL="http://"+ip+":"+port+"/api/";
string Login_Url=API_URL+"unlock?passwd="+passwd;
string Finish_URL=API_URL+"finishedtask";
string Working_URL=API_URL+"workingtask";
string Delete_Url=API_URL+"deletetask";
string logincmd="curl -c temp.txt "+Login_Url;
string status=getcmd(logincmd);
cout<<"debug:"<<status<<endl;
if(status!="{\"code\":200}"){
        cout<<"Login Fail"<<endl;
        exit(1);
}
cout<<"Login Success"<<endl;
string Finish_Cmd="curl -b temp.txt "+Finish_URL;
string Finish_Task=getcmd(Finish_Cmd);
string searchid="\"file_id\":\"";
string searchname="\"file_name\":\"";
int static_index=0;
queue<dict> result;
while(static_index<Finish_Task.length()){
int index=getstr(Finish_Task,searchid,static_index);
if(index==-1){
break;
}
string ids="";
for(int i=index;i<Finish_Task.length();i++){
        if(Finish_Task[i]!='\"'){
        ids+=Finish_Task[i];
        }
        if(Finish_Task[i]=='\"'){
                break;
        }
}
int name_index=getstr(Finish_Task,searchname,index);
if(name_index==-1){
break;
}
string filename="";
for(int j=name_index;j<Finish_Task.length();j++){
if(Finish_Task[j]!='\"'){
filename+=Finish_Task[j];
}
if(Finish_Task[j]=='\"'){
static_index=j;
break;
}
}
dict a(ids,filename);
a.show();
result.push(a);
}
int queuelength=result.size();
for(int i=0;i<queuelength;i++){
string current_time=getcmd("date \"+\%Y\%m\%d\%H\%M\%S\"");
string filename=result.front().getvalue();
string filekey=result.front().getkey();
string filedir=current_time+result.front().getvalue();
for(int i=0;i<filedir.length();i++){
if(filedir[i]=='\n'){
filedir[i]='-';
}
}
string cmd1="mkdir -p \'/temp/"+filedir+"\'";
string cmd2="mv \'"+downloaddir+"/"+filename+"\' \'/temp/"+filedir+"/"+filedir+"\'";
string cmd3="python \'"+uploadprogram+"\' UploadDir"+" \'/temp/"+filedir+"\' "+uploaddir+" "+panf;
string cmd4="rm -rf \'/temp/"+filedir+"\'";
string cmd5="rm -rf \'"+downloaddir+"/mask/\'";
string cmd6="python \'" + uploadprogram+"\' UpdateFile";
string cmd7="curl -b temp.txt -d \"ids=[\\\""+filekey+"\\\"]&delfile=1\" "+Delete_Url;
cout<<"start"<<endl;
if(system(cmd1.data())!=0){
cout<<"create temp dir fail !"<<endl;
}

if(system(cmd2.data())!=0){
cout<<"move fail !"<<endl;
}
if(system(cmd7.data())){
cout<<"post fail !"<<endl;
}
if(system(cmd3.data())!=0){
cout<<"upload fail !"<<endl;
}
if(system(cmd4.data())!=0){
cout<<"rm fail !"<<endl;
}
if(system(cmd5.data())!=0){
cout<<"rm mask fail !"<<endl;
}
if(system(cmd6.data())!=0){
cout<<"update fail"<<endl;
}
cout<<filename+" upload success !"<<endl;

result.pop();
}


//end
}

int main(int argc,char * argv[])
{
string ip="";
string port="";
string passwd="";
for(int i = 1;i< argc;i++)
{
	string b=argv[i];
	if(b=="help"){
		cout << "./renrenupload renren ip port password uploadprogram uploaddir downloaddir panfu" <<endl;
		cout<<"帮助\n参数说明:\n1.renren:指定人人影视，后期会加入aria2\n2.ip 为ip地址\n3.port 为端口号\n4.password 为人人影视解锁码\n5.uploadprogram为上传函数的路径 \n6.uploaddir为上传onedrive位置\n7.downloaddir为你下载的路径\n8.panfu 为上传的盘符\n"<<endl;
		cout<<"例如:./renrenupload renren 127.0.0.1 3001 123456 /root/PyOne/function.py / /root/PyOne/upload A \n注意uploadprogram路径精确到文件名，downloaddir路径最后不要加/\n"<<endl;
		break;
	}
}
string select=argv[1];
if(select=="renren"){
ip=argv[2];
port=argv[3];
passwd=argv[4];
string uploadprogram=argv[5];
string uploaddir=argv[6];
string downloaddir=argv[7];
string panf=argv[8];
renren(ip,port,passwd,uploadprogram,uploaddir,downloaddir,panf);
}
return 0;
}
