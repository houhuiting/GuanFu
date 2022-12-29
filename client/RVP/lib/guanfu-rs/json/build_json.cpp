#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cassert>
#include "include/json/json.h"

#define CMD_RESULT_BUF_SIZE 102400
#define Package_SIZE 1024
#define binary_length 100

using namespace std;


/*
 * cmd：待执行命令
 * result：命令输出结果
 * 函数返回：0 成功；-1 失败；
 */
int ExecuteCMD(const char *cmd, char *result)
{
    int iRet = -1;
    char buf_ps[CMD_RESULT_BUF_SIZE];
    char ps[CMD_RESULT_BUF_SIZE] = {0};
    FILE *ptr;

    strcpy(ps, cmd);

    if((ptr = popen(ps, "r")) != NULL)
    {
        while(fgets(buf_ps, sizeof(buf_ps), ptr) != NULL)
        {
           strcat(result, buf_ps);
           if(strlen(result) > CMD_RESULT_BUF_SIZE)
           {
               break;
           }
        }
        pclose(ptr);
        ptr = NULL;
        iRet = 0;  // 处理成功
    }
    else
    {
        printf("popen %s error\n", ps);
        iRet = -1; // 处理失败
    }

    return iRet;
}

//判断该包是否在文件中，并将该包存放的github位置返回
string exist_package(string file, string PackageName)
{
    //将文件打开
    string s;
    // 文件流对象
    ifstream infile;
    //把文件流对象和文件连接起来
    infile.open(file.data());
    //如果连接失败，则抛出错误
    assert(infile.is_open());

    while (getline(infile, s))
    {
        // 查找该包是否在文件中
        string::size_type idx;
        idx = s.find(PackageName);
        if (idx != string::npos )
        {
            int url_idx_begin = s.find("https", idx);
            int url_idx_end = s.find(".git", url_idx_begin);
            string url = s.substr(url_idx_begin, url_idx_end + 4 - url_idx_begin);
            return url;
        }
    }
    infile.close();
    return "0";
}


//主函数
int main()
{
    //初始化待写入json文件的信息
    map<string, string> map_property;
    map<string, string >map_dependency;

    //输入包名
    vector<string> package_list;
    package_list.push_back("fast_socks5");
    package_list.push_back("rsass");
    package_list.push_back("ic_cdk");
    package_list.push_back("color_name");
    package_list.push_back("loggerv");
    package_list.push_back("powershell_script");
    package_list.push_back("wl_clipboard_rs");
    package_list.push_back("paris");
    package_list.push_back("pelite");
    package_list.push_back("streampager");

    for (int i = 0; i < package_list.size(); i++) {
        string PackageName = package_list[i];
        map_property["name"] = PackageName;

        string fliename = "old.guanfu_" + PackageName;

        //获取sourceLink
        map_property["sourceLink"] = exist_package("lib_and_link.txt", PackageName);
        //判断输入的包，是否在new_rust.txt文件中
        if (map_property["sourceLink"] == "0")
        {
            cout <<"have not this Package\n";
            return 0;
        }


        //获取version_gf
        map_property["version_gf"] = "0.1";

        //获取architecture
        map_property["architecture"] = "x86-64";

        //获取kernel_version
        map_property["kernel_version"] = "5.0";

        //获取timestamp
        map_property["timestamp"] = "1234567890";

        //获取version
        map_property["version"] = "1.0";

        //获取sourceType
        map_property["sourceType"] = "git";

        //获取二进制文件存放的url
        map_property["url"] = "/" + PackageName + "/target/debug/" + "lib" + PackageName + ".rlib";
        //map_property["url"] = "/" + PackageName + "/target/debug/" + PackageName;

        //获取md5，因为此md5是构建好的二进制文件的md5，这里不清楚，设置为空
        map_property["md5"] = " ";

        //获取sha1，因为此sha1是构建好的二进制文件的sha1，这里不清楚，设置为空
        map_property["sha1"] = " ";

        //获取sha256，因为此sha256是构建好的二进制文件的sha256，这里不清楚，设置为空
        map_property["sha256"] = " ";

        //获取command
        vector<string> map_command(5);
        map_command[0] = "rustup target add x86_64-unknown-linux-musl";
        map_command[1] = "git clone " + map_property["sourceLink"] + " " + PackageName + "/";
        map_command[2] = "cd /home/code/" + PackageName;
        map_command[3] = "cargo fetch";
        map_command[4] = "dettrace --base-env host -- cargo build";

        // 获取依赖项
        map_dependency["make"] = "4.1-9.1ubuntu1";
        map_dependency["rust"] = "1.66.0";
        map_dependency["git"] = "1:2.17.1-1ubuntu0.13";
        map_dependency["curl"] = "7.58.0-2ubuntu3.20";
        map_dependency["libseccomp-dev"] = "2.5.1-1ubuntu1~18.04.2";
        map_dependency["gcc"] = "4:7.4.0-1ubuntu2.3";
        map_dependency["pkg-config"] = "0.29.1-0ubuntu2";
        map_dependency["libssl-dev"] = "1.1.1-1ubuntu2.1~18.04.20";

        // 初始化json
        Json::Value frame;
        Json:: StyledWriter styledWriter;
        Json::Value config, name, version_gf, source, environment, dependency, command,  binary, timestamp;
        Json::Value version;
        Json::Value sourceType, sourceLink;

        Json::Value architecture, kernel_version;
        Json::Value source_dep, version_dep;
        Json::Value url, checksum, md5, sha1, sha256;
        Json::Value bin,dep;

        config["version_gf"] = map_property["version_gf"];
        frame["config"] = config;
        frame["name"] = map_property["name"] ;
        frame["version"] = map_property["version"];
        source["sourceType"] = map_property["sourceType"];
        source["sourceLink"] =  map_property["sourceLink"] ;
        frame["source"] = source;
        environment["architecture"] = "x86-64";
        environment["kernel_version"] = "5.0";
        frame["environment"] = environment;
        // 写入依赖
        {
            map<string, string>::iterator it = map_dependency.begin();
            while(it!= map_dependency.end())
            {   
                dep["source_dep"] = it->first;
                dep["version_dep"]= it->second;
                dependency.append(dep);
                it++;            
            }
        }
        frame["dependency"] = dependency;
    
        // 写入命令
        for (int j = 0; j < 5; j++)
        {
            command.append(map_command[j]);
        }


        frame["command"] = command;
        bin["url"] = map_property["url"];
        checksum["md5"] =map_property["md5sum"];
        checksum["sha1"]= map_property["shasum"];
        checksum["sha256"]= map_property["sha256sum"];
        bin["checksum"] = checksum;
        frame["binary"] = bin;
        frame["timestamp"] = map_property["timestamp"];


        string result = styledWriter.write(frame);
        ofstream write_file(fliename);
        write_file<<result.c_str()<<endl;
        write_file.close();
    }
    


    // //如果该包在new_rust.txt文件中，生成查看其包信息的命令 
    // char result[CMD_RESULT_BUF_SIZE]={0};
    // char com_getInfor[CMD_RESULT_BUF_SIZE] = "apt-cache show ";
    // strcat(com_getInfor, PackageName);
    // cout << com_getInfor << endl;
    // // 将包信息存到result中
    // ExecuteCMD(com_getInfor, result);
    // //将包信息result的格式转换为string，包信息放入pack_infor
    // string pack_infor(result);

    // //输入包信息
    // cout << "This is an example\n\n";
    // cout << pack_infor;
    // cout << "\n\nThis is end\n";

    

    return 0;


}