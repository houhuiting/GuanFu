#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cassert>

#define CMD_RESULT_BUF_SIZE 1024000
#define Package_SIZE 1024

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

int ExecuteCMD(const char *cmd)
{
    int iRet = -1;
    char buf_ps[CMD_RESULT_BUF_SIZE];
    char ps[CMD_RESULT_BUF_SIZE] = {0};
    FILE *ptr;

    strcpy(ps, cmd);

    if((ptr = popen(ps, "r")) != NULL)
    {
        return 1;
    }

    return 0;
}


//主函数
int main()
{
    // 以写模式打开文件
    ofstream outfile;
    outfile.open("beach_rebuild.dat");

    vector<string> package_list;
    // package_list.push_back("fast_socks5");
    // package_list.push_back("rsass");
    // package_list.push_back("ic_cdk");
    package_list.push_back("color_name");
    package_list.push_back("loggerv");
    package_list.push_back("powershell_script");
    package_list.push_back("wl_clipboard_rs");
    package_list.push_back("paris");
    package_list.push_back("pelite");
    package_list.push_back("streampager");
    
    char cmd[CMD_RESULT_BUF_SIZE];

    for (int i = 0; i < package_list.size(); i++)
    {
        // 第一次生成old.guanfu文件
        printf("11111111111111111111111111111111111111\n");
        string com = "cp guanfu-rs/mount/old.guanfu_" + package_list[i] + " guanfu-rs/mount/old.guanfu";
        strcpy(cmd, com.c_str());
        if (ExecuteCMD(cmd))
        {
            cout << package_list[i] << " first old.guanfu successfully created" << endl;
        }
        else
        {
            return 0;
        }
        
        // 第一次执行构建，不断做循环，直到第一次构建成功
        int loop_num = 0;
        int m = -1;
        while (m == -1)
        {
            printf("22222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222\n");
            char result1[CMD_RESULT_BUF_SIZE]={0};
            strcpy(cmd, "bash guanfu-rs/build.sh");
            ExecuteCMD(cmd, result1);
            string str_result1(result1);
            cout << str_result1 <<endl;
            m = str_result1.find("Sorry for the checksum inconsistency");
            loop_num = loop_num + 1;
            // 如果10次都没有成功，说明大概率不成功，记入日志中
            if (loop_num >= 10)
            {
                outfile << package_list[i] << " executed 10 times, still can't build" << endl;
                break;
            }
        } 
        // 如果第一次构建执行了10次不成功，说明代码大概率不能构建，直接continue掉，执行下一个数据包
        if (loop_num >= 10)
        {
            outfile << "——————————————————————————————————————————————————————————————\n" << endl;
            continue;
        }
        outfile << package_list[i] << " first build successfully spend "<< loop_num << " times" << endl;
        cout << "first build successfully!" << endl;
        
        // 将第一次执行构建得到的new.guanfu，作为old.guanfu文件进行下一次构建
        printf("3333333333333333333333333333333333333333333333333333333333333333333333333333\n");
        com = "cp guanfu-rs/mount/new.guanfu guanfu-rs/mount/new.guanfu_" + package_list[i];
        strcpy(cmd, com.c_str());
        ExecuteCMD(cmd);
        com = "cp guanfu-rs/mount/new.guanfu guanfu-rs/mount/old.guanfu";
        strcpy(cmd, com.c_str());
        ExecuteCMD(cmd);
        cout << "Rename new.guanfu to old.guanfu" << endl;

        // 第二次构建
        int n = -1;
        loop_num = 0;
        while (n == -1)
        {
            printf("4444444444444444444444444444444444444444444444444444444444444444444444444\n");
            loop_num += 1;
            char result2[CMD_RESULT_BUF_SIZE]={0};
            strcpy(cmd, "bash guanfu-rs/build.sh");
            // 将包信息存到result中
            ExecuteCMD(cmd, result2);
            //将result的格式转换为string
            string str_result2(result2);
            cout << str_result2 <<endl;
            bool flag = false;
            if((n = str_result2.find("This is a repeatable build.")) != string::npos)
	        {
		        flag = true;
                outfile << package_list[i] << " successfully execute reproducible build code" << endl;
	        }
            else
            {
                flag = false;
                outfile << package_list[i] << " fail execute reproducible build code" << endl;
            }
            // 如果10次都没有成功，说明大概率不成功，记入日志中
            if (loop_num >= 10)
            {
                outfile << package_list[i] << "executed 10 times, still can't repeat the build" << endl;
                break;
            }
        }
        outfile << "——————————————————————————————————————————————————————————————\n" << endl;
        
        
    }
    
    outfile.close();


    return 0;


}