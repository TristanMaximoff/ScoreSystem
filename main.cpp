//头文件以及宏
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MAX_STUDENT 100  //最大学生数  目前为100，可在此处修改
#define SUBJECT_CP  1    //长跑标识
#define SUBJECT_LDTY 2   //立定跳远标识
#define SUBJECT_SXQ 3    //实心球标识

//学生结构体以及学生.成绩结构体定义
typedef struct Score {
    float cp;     // 长跑
    float ldty;   // 立定跳远
    float sxq;    // 实心球
    float total;  // 总分
} Score;
typedef struct Student {
    char cla[20];   // 班级
    char num[10];   // 学号
    char name[20];  // 姓名
    Score score;    // 成绩
    char grade[10]; // 总评（EXCELLENT/GOOD/PASS/FAIL）
} Student;

// 函数声明（在word和后文函数定义中有功能解释）
// 菜单函数
void mainMenu(int isLog);
void querySubMenu();
void batchSubMenu();
// 基础功能
int saveToFile(Student stuArr[],int count);   
int loadFromFile(Student stuArr[],int maxSize);
void showAllStudent(Student stuArr[],int count); 
void inputStudent(Student *stu);
void inputScore(Student *stu);
void sortGrade(Student *stu); 
int logIn();
// 排序功能
void sortByName(Student stuArr[],int count);          
void sortByTotal(Student stuArr[],int count);         
void sortByNum(Student stuArr[],int count);           
void sortByClass(Student stuArr[],int count);         
void sortByClassThen(Student stuArr[],int count);     
void sortBySubject(Student stuArr[],int count,int subject); 
// 查询功能
void queryFailedStudent(Student stuArr[],int count);  
void queryClassTotalRank(Student stuArr[],int count); 
void querySubjectRank(Student stuArr[],int count);    
// 删除/修改功能
int deleteStudent(Student stuArr[],int *count);       
int modifyStudent(Student stuArr[],int count);       
// 批量操作功能
void batchAddStudent(Student stuArr[],int *count);    
void batchModifyStudent(Student stuArr[],int count);  
void batchDeleteStudent(Student stuArr[],int *count); 

//全局变量
Student stuArr[MAX_STUDENT];   //数组目前空间为100，修改在上文的宏
int stuCount=0;

//main函数
int main() {
    // 启动时加载历史数据
    stuCount=loadFromFile(stuArr, MAX_STUDENT);            //读取文件后将学生数据数量写入stuCount
    printf("系统启动成功，已加载 %d 条历史数据！\n", stuCount);

    int choice;
    int isLog=0;    //0为游客模式，1为教师登录
    while(1){
        mainMenu(isLog);
        
        printf("请输入选项："); scanf("%d",&choice);

        switch(choice){
            case 1://教师：录入单个学生信息   游客：登录
                if(isLog==0){
                    isLog=logIn();
                    if(isLog==0) printf("登录失败!\n");
                    else printf("登录成功!\n");
                    break;
                }else{
                    if(stuCount>=MAX_STUDENT){
                    printf("学生数量已达上限%d！\n",MAX_STUDENT);
                    break;
                    }
                    printf("\n----- 录入单个学生 -----\n");
                    inputStudent(&stuArr[stuCount]);
                    stuCount++;
                    saveToFile(stuArr,stuCount); // 自动保存
                    break;
                }
                
                

            case 2://展示全部成绩
                showAllStudent(stuArr,stuCount);
                break;

            case 3://查询功能
                while(1)
                {
                    querySubMenu();
                    int qChoice; printf("输入查询选项："); scanf("%d", &qChoice);
                    switch (qChoice) {
                        case 1: queryFailedStudent(stuArr,stuCount); break;
                        case 2: queryClassTotalRank(stuArr,stuCount); break;
                        case 3: querySubjectRank(stuArr,stuCount); break;
                        case 0: goto QUIT_QUERY; // 退出查询子菜单
                        default: printf("输入错误！\n");
                    }
                }
                QUIT_QUERY:
                break;

            case 4: // 删除学生
                if(isLog==0){
                    printf("输入错误，请重新选择！\n");
                    break;
                }else{
                    deleteStudent(stuArr, &stuCount);
                    break;
                }
                

            case 5: // 修改学生
            if(isLog==0){
                printf("输入错误，请重新选择！\n");
                break;
                }else{
                    modifyStudent(stuArr, stuCount);
                break;
                }

            case 6: // 批量操作
                if(isLog==0){
                    printf("输入错误，请重新选择！\n");
                    break;
                }else{
                    while (1) {
                    batchSubMenu();
                    int bChoice; printf("输入批量操作选项："); scanf("%d", &bChoice);
                    switch (bChoice) {
                        case 1: batchAddStudent(stuArr,&stuCount); break;
                        case 2: batchModifyStudent(stuArr,stuCount); break;
                        case 3: batchDeleteStudent(stuArr,&stuCount); break;
                        case 0: goto QUIT_BATCH; // 退出批量子菜单
                        default: printf("输入错误！\n");
                    }
                }
                QUIT_BATCH:
                break;
                }
            
            case 7:  //退出教师账号
                if(isLog==0){
                    printf("输入错误，请重新选择！\n");
                    break;
                }
                isLog=0;
                printf("已退出教师系统\n");
                break;
                
            case 0: // 退出系统
                printf("系统已退出！\n");
                return 0;

            default:
                printf("输入错误，请重新选择！\n");
        }
        printf("\n"); // 空行分隔，优化交互
    }
    return 0;
}

//函数定义
// 1. 菜单函数
//主菜单
void mainMenu(int isLog)
 {
    printf("\n"); //美观
    printf("===================== 体测成绩管理系统 =====================\n");
    if(isLog==0){
        printf("当前为游客系统：\n");
        printf("1.教师登录\n");
        printf("2.展示全部成绩\n");
        printf("3.查询功能\n");
    }else if(isLog==1){
        printf("当前为教师系统：\n");
        printf("1.录入单个学生成绩\n");
        printf("2.展示全部成绩\n");
        printf("3.查询功能\n");
        printf("4.删除指定学生成绩\n");
        printf("5.修改指定学生成绩\n");
        printf("6.批量操作（追加/修改/删除）\n");
        printf("7.退出教师账号\n");
    }else{
        printf("登录信息有误，请退出系统重进\n");
        return;
    }
    printf("0.退出系统\n");
    printf("==========================================================\n");
    printf("\n");
}
//主菜单-查询子菜单
void querySubMenu()
 {
    printf("\n");
    printf("===================== 查询子菜单 =====================\n");
    printf("1.查询任一科目不及格学生\n");
    printf("2.查询各班总分前n名/最低n名\n");
    printf("3.查询指定科目前n名/最低n名\n");
    printf("0.返回主菜单\n");
    printf("======================================================\n");
    printf("\n");
}
//主菜单-批量菜单
void batchSubMenu()
 {
    printf("\n");
    printf("===================== 批量操作子菜单 =====================\n");
    printf("1. 批量追加学生数据\n");
    printf("2. 批量修改指定学生成绩\n");
    printf("3. 批量删除指定学生\n");
    printf("0. 返回主菜单\n");
    printf("==========================================================\n");
    printf("\n");
}

// 2. 基础功能函数
//录入一个学生成绩，不自动保存
void inputStudent(Student *stu)
{
    printf("班级：");
    scanf("%s",stu->cla);
    printf("学号：");
    scanf("%s",stu->num);
    printf("姓名：");
    scanf("%s",stu->name);

    inputScore(stu);
    sortGrade(stu);  //读取三个成绩，把等级存入结构体
}
//通用成绩录入/修改函数（可在单独录入和批量修改/增添中用到）
void inputScore(Student *stu)
{
    float s1,s2,s3,max;
    stu->score.total=0.0;

    printf("三次长跑成绩（用逗号隔开）");
    scanf("%f,%f,%f",&s1,&s2,&s3);
    if(s1<0||s1>100||s2<0||s2>100||s3<0||s3>100){
        printf("分数不在范围内！\n");
        return;
    }
    max=MAX(MAX(s1,s2),s3);
    stu->score.cp=max;
    stu->score.total+=max;

    printf("三次立定跳远成绩（用逗号隔开）");
    scanf("%f,%f,%f",&s1,&s2,&s3);
    if(s1<0||s1>100||s2<0||s2>100||s3<0||s3>100){
        printf("分数不在范围内！\n");
        return;
    }
    max=MAX(MAX(s1,s2),s3);
    stu->score.ldty=max;
    stu->score.total+=max;

    printf("三次实心球成绩（用逗号隔开）");
    scanf("%f,%f,%f",&s1,&s2,&s3);
    if(s1<0||s1>100||s2<0||s2>100||s3<0||s3>100){
        printf("分数不在范围内！\n");
        return;
    }
    max=MAX(MAX(s1,s2),s3);
    stu->score.sxq=max;
    stu->score.total+=max;
}
//输入结构体，评级
void sortGrade(Student *stu)
{
    if (stu->score.cp >= 89.5 && stu->score.ldty >= 89.5 && stu->score.sxq >= 89.5) {
        strcpy(stu->grade, "EXCELLENT");
    } else if (stu->score.cp >= 79.5 && stu->score.ldty >= 79.5 && stu->score.sxq >= 79.5) {
        strcpy(stu->grade, "GOOD");
    } else if (stu->score.cp >= 59.5 && stu->score.ldty >= 59.5 && stu->score.sxq >= 59.5) {
        strcpy(stu->grade, "PASS");
    } else {
        strcpy(stu->grade, "FAIL");
    }
}
//w-清空文件后重新讲stuArr写入score.txt
int saveToFile(Student stuArr[],int count) 
{
    FILE *fp=fopen("score.txt","w");  //w-清空文件内内容再写入新数据
    
    if (fp==NULL){
        printf("文件打开失败！\n");
        return 0;
        }
    
    int i;
    for (i=0;i<count; i++) { //每次保存文件时是清空文件数据后，历遍从0-stuCount-1（函数内味count）的stuArr重新写入文件
        fprintf(fp, "%s %s %s %.1f %.1f %.1f %.1f %s\n",
                stuArr[i].cla,stuArr[i].num,stuArr[i].name,
                stuArr[i].score.cp,stuArr[i].score.ldty,stuArr[i].score.sxq,
                stuArr[i].score.total,stuArr[i].grade);
    }
    fclose(fp);
    
    printf("数据已保存到score.txt\n");
    return 1;
}
//在每次启动程序前先读取score.txt中的数据存进stuArr中
int loadFromFile(Student stuArr[],int maxSize) //maxsize传入MAX_STUDENT的宏100
{
    FILE *fp = fopen("score.txt","r");
    
    if (fp == NULL) {
        printf("暂无历史数据\n");
        return 0;
    }

    int count;
    for (count=0; count<maxSize; count++) {
        int readResult;
        readResult=fscanf(fp,"%s %s %s %f %f %f %f %s",
                        stuArr[count].cla,stuArr[count].num,stuArr[count].name,
                        &stuArr[count].score.cp,&stuArr[count].score.ldty,&stuArr[count].score.sxq,
                        &stuArr[count].score.total, stuArr[count].grade);

        if (readResult==EOF){           //当没读取到东西时fscanf返回-1，即EOF宏
            break;
        }
    }

    fclose(fp);
    return count;             //返回当前文件中有的学生数量
}
//printf所有学生数据
void showAllStudent(Student stuArr[],int count)        //count由stuCount传入
 {
    if (count==0){
        printf("暂无学生数据\n");
        return;
        }
    
    // 排序方式选择
    printf("\n");
    printf("请选择排序方式：\n");
    printf("1. 按姓名字母排序\n");
    printf("2. 按总分降序\n");
    printf("3. 按学号排序\n");
    printf("4. 按班级排序（可二次排序）\n");
    printf("5. 按指定科目排序\n");
    printf("\n");
    
    int choice;
    scanf("%d",&choice);
    
    switch (choice) {
        case 1: sortByName(stuArr,count); break;
        case 2: sortByTotal(stuArr,count); break;
        case 3: sortByNum(stuArr,count); break;
        case 4: sortByClassThen(stuArr,count); break;
        case 5: 
            printf("输入数字选择科目（1-长跑 2-立定跳远 3-实心球）：");
            int sub;   //用int sub判断，前文宏定义了三种运动
            scanf("%d",&sub);
            sortBySubject(stuArr,count,sub);
            break;
        default:
            printf("输入错误，默认按总分排序\n");
            sortByTotal(stuArr, count);
    }

    // 展示数据
    printf("=======================================所有学生成绩=======================================\n");
    printf("%-10s %-10s %-15s %-5s %-10s %-5s %-5s %-10s\n","班级","学号","姓名","长跑","立定跳远","实心球","总分","总评");
    printf("-----------------------------------------------------------------------------------------\n");
    //利用-n左对齐，美观
    int i;
    for(i=0;i<count;i++){
	    printf("%-10s %-10s %-15s %-5.1f %-10.1f %-5.1f %-5.1f %-10s\n",
        stuArr[i].cla,stuArr[i].num,stuArr[i].name,
        stuArr[i].score.cp,stuArr[i].score.ldty,stuArr[i].score.sxq,
        stuArr[i].score.total,stuArr[i].grade);
        }
}
//登录系统,账号密码信息需提前存在account.txt中
int logIn(){
    FILE *fp=fopen("account.txt","r");
    if(fp==NULL){
        printf("账号文件打开失败\n");
        return 0;
    }
    char acc[15]="",pw[15]="";         //输入的账号密码
    char acc1[15]="",pw1[15]="";        //从文件中对比的账号密码
    
    printf("请输入您的账号:");
    scanf("%s",acc);
    printf("请输入您的密码:");
    scanf("%s",pw);
    
    int isCheck=0;                    //为0表示没找到相应的账号密码
    while(fscanf(fp,"%s %s",acc1,pw1)!=EOF){
        if((strcmp(acc1,acc)==0)&&(strcmp(pw,pw1)==0)){
            isCheck=1;
            break;
        }
    }
    
    fclose(fp);
    return isCheck;
}
// 3. 排序功能函数
void sortByName(Student stuArr[],int count)
{
    int i,j;
    for(i=0;i<count-1;i++){
        for(j=0;j<count-1-i;j++){
            if(strcmp(stuArr[j].name,stuArr[j+1].name)>0){
                Student temp;
                temp=stuArr[j];
                stuArr[j]=stuArr[j+1];
                stuArr[j+1]=temp;
            }
        }
    }
}

void sortByTotal(Student stuArr[],int count)
{
    int i,j;
    for(i=0;i<count-1;i++){
        for (j=0;j<count-1-i;j++){
            if(stuArr[j].score.total<stuArr[j+1].score.total){
                Student temp;
                temp=stuArr[j];
                stuArr[j]=stuArr[j+1];
                stuArr[j+1]=temp;
            }
        }
    }
}

void sortByNum(Student stuArr[],int count) 
{
    int i,j;
    for(i=0;i<count-1;i++){
        for (j=0;j<count-1-i;j++){
            if (strcmp(stuArr[j].num,stuArr[j+1].num)>0){
                Student temp;
                temp=stuArr[j];
                stuArr[j]=stuArr[j+1];
                stuArr[j+1]=temp;
            }
        }
    }
}

void sortByClass(Student stuArr[],int count) 
{
    int i,j;
    for (i=0;i<count-1;i++){
        for(j=0;j<count-1-i;j++){
            if(strcmp(stuArr[j].cla,stuArr[j+1].cla)>0){
                Student temp;
                temp=stuArr[j];
                stuArr[j]=stuArr[j+1];
                stuArr[j+1]=temp;
            }
        }
    }
}
//按照班级排序扎堆后二次排序
void sortByClassThen(Student stuArr[], int count)
{
    int i,j,choice;
    Student temp;
    sortByClass(stuArr,count);
    
    printf("班级内部排序方式：\n");
    printf("1.姓名字母\n2.学号\n3.总分降序\n");
    printf("请选择：");
    scanf("%d",&choice);

    for(i=0;i<count-1;i++){
        for(j=0;j<count-1-i;j++){
            if(strcmp(stuArr[j].cla, stuArr[j+1].cla) != 0) continue;
            if(choice==1){
                if(strcmp(stuArr[j].name,stuArr[j+1].name)>0){
                    temp=stuArr[j];
                    stuArr[j]=stuArr[j+1];
                    stuArr[j+1]=temp;
                    }
            }else if(choice==2){
                if(strcmp(stuArr[j].num,stuArr[j+1].num) > 0){
                    temp=stuArr[j];
                    stuArr[j]=stuArr[j+1];
                    stuArr[j+1]=temp;
                }
            }else if(choice==3){
                if(stuArr[j].score.total<stuArr[j+1].score.total){
                    temp=stuArr[j];
                    stuArr[j]=stuArr[j+1];
                    stuArr[j+1]=temp;
                }
            }
        }
    }
}
//选一科，根据分数排序
void sortBySubject(Student stuArr[],int count,int subject) //subject 1 2 3分别代表长跑，立定跳远实心球
{
    int i,j;
    for(i=0;i<count-1;i++){
        for (j=0;j<count-1-i;j++){
            float scoreJ = 0, scoreJ1 = 0;
            switch (subject){
                case SUBJECT_CP:scoreJ=stuArr[j].score.cp;scoreJ1=stuArr[j+1].score.cp; break;       //宏实际为1,2,3
                case SUBJECT_LDTY:scoreJ=stuArr[j].score.ldty; scoreJ1=stuArr[j+1].score.ldty; break;
                case SUBJECT_SXQ:scoreJ=stuArr[j].score.sxq; scoreJ1=stuArr[j+1].score.sxq; break;
            }
            if (scoreJ < scoreJ1) {            //代码简洁，不然要在每个case里都交换一遍
                Student temp=stuArr[j];
                stuArr[j]=stuArr[j+1];
                stuArr[j+1]=temp;
            }
        }
    }
}

// 4. 查询功能函数
//写入任意一科不及格的学生数据，并在程序里展现出来
void queryFailedStudent(Student stuArr[],int count) 
{
    FILE *fp = fopen("bujige.txt", "w");
    if (fp==NULL){
        printf("文件打开失败\n");
        return;
        }
    fprintf(fp, "不及格学生列表（任一科目<59.5）：\n");
    fprintf(fp, "班级\t学号\t姓名\t长跑\t立定跳远\t实心球\t不及格科目\n");
    fprintf(fp, "--------------------------------------------------------\n");
    
    int i;
    for (i=0;i<count;i++){
        char failedSub[50] = "";
        int isFailed=0;           //至少有一科不及格的话isFail=1并且写入文件
        
        if(stuArr[i].score.cp < 59.5){
            strcat(failedSub,"长跑 ");
            isFailed=1;
            }
        if(stuArr[i].score.ldty<59.5){
            strcat(failedSub,"立定跳远 ");
            isFailed=1;
            }
        if(stuArr[i].score.sxq < 59.5){
            strcat(failedSub,"实心球 ");
            isFailed=1;
            }

        if (isFailed==1){
            fprintf(fp,"%s\t%s\t%s\t%.1f\t%.1f\t\t%.1f\t%s\n",
                    stuArr[i].cla, stuArr[i].num, stuArr[i].name,
                    stuArr[i].score.cp, stuArr[i].score.ldty,
                    stuArr[i].score.sxq, failedSub);
            printf("%s\t%s\t%s\t%.1f\t%.1f\t\t%.1f\t%s\n",
                    stuArr[i].cla, stuArr[i].num, stuArr[i].name,
                    stuArr[i].score.cp, stuArr[i].score.ldty,
                    stuArr[i].score.sxq, failedSub);
        }
    }
    fclose(fp);
    printf("不及格数据已保存到bujige.txt！\n");
}
//先分班，再根据总分排序，输出前n名学生
void queryClassTotalRank(Student stuArr[],int count)
{
    int start;              //用来标记每次输出时的起点，例如后n名就是从
    int n;
    printf("请输入查询人数n："); 
    scanf("%d", &n);
    if(n<=0||n>count){
        printf("n输入无效！\n");
        return;
    }

    //先根据班级排序后，再利用和sortByClassThen一样的逻辑进行班内扎堆的总分排序      
    sortByClass(stuArr,count);
    int i,j;
    Student temp;
    for(i=0;i<count-1;i++){
        for(j=0;j<count-1-i;j++){
            if(strcmp(stuArr[j].cla,stuArr[j+1].cla)!=0) continue;
            if(stuArr[j].score.total<stuArr[j+1].score.total){
                temp=stuArr[j];
                stuArr[j]=stuArr[j+1];
                stuArr[j+1]=temp;
            }
        }
    }

    char curClass[20] = "";
    int classStart=0;         //classStart用来标记每个班从第i个stuArr[i]开始
    printf("===================== 各班总分排名 =====================\n");
    for(i=0;i<count;i++){
        //每当班级变了，从班级第一个数据所在的i开始历遍，把前n后n名输出，之后一直跳过同一个班直到下一个班
        if (strcmp(stuArr[i].cla, curClass)!=0){  //保证在同一个班内，防止当n>本班人数导致触碰下一个班级的情况
            
            if(i>0){                            //如果是开始就先跳过，先将curClass覆盖成第一个class
                printf("【%s 总分最低%d名】\n", curClass,n);   
                if(i-n<classStart) start=classStart;
                else start=i-n;
                for (j=start;j<i;j++){
                    printf("%s\t%s\t%s\t总分：%.1f\n",
                           stuArr[j].cla, stuArr[j].num, stuArr[j].name, stuArr[j].score.total);
                }
            }
            
            strcpy(curClass,stuArr[i].cla);  //更新curClass成下一个
            classStart=i;
            printf("【%s 总分最高%d名】\n",curClass,n);
            int end;
            if(i+n>count) end=count;    //保证到最后一组班级时不跨越STUDENT_MAX
            else end=i+n;
            for(j=i;j<end;j++){
                printf("%s\t%s\t%s\t总分：%.1f\n",
                       stuArr[j].cla,stuArr[j].num,stuArr[j].name,stuArr[j].score.total);
            }
        }
    }
    printf("【%s 总分最低%d名】\n",curClass,n);

    if((count-n)<classStart) start=classStart;
    else start=count-n;
    
    for (j=start;j<count;j++){
        printf("%s\t%s\t%s\t总分：%.1f\n",
               stuArr[j].cla, stuArr[j].num, stuArr[j].name, stuArr[j].score.total);
    }
}
//查询某一科分数前n人
void querySubjectRank(Student stuArr[], int count) 
{
    int subject,n;
    printf("选择科目：1-长跑 2-立定跳远 3-实心球：");
    scanf("%d", &subject);    //用来储存决策
    printf("输入查询人数n：");
    scanf("%d", &n);
    if ((n<=0)||(n>count)||(subject<1)||(subject>3)){
        printf("输入无效！\n"); 
        return;
    }

    sortBySubject(stuArr,count,subject);   //先对特定科目分数进行排序
    char subName[20]="";                   //说明是哪一科
    switch(subject){
        case 1:strcpy(subName,"长跑"); break;
        case 2:strcpy(subName,"立定跳远"); break;
        case 3:strcpy(subName,"实心球"); break;
    }

    printf("\n");
    printf("【%s 成绩最高%d名】\n",subName,n);
    int i;
    for(i = 0; i < n; i++) {
        float score;                //根据选择赋值并输出score，代码简洁
        if(subject==1){
            score=stuArr[i].score.cp;
        }else if(subject==2){
            score=stuArr[i].score.ldty;
        }else{
            score=stuArr[i].score.sxq;
        }
        printf("%s\t%s\t%s\t%s成绩：%.1f\n",
               stuArr[i].cla,stuArr[i].num,stuArr[i].name,subName,score);
    }

    printf("【%s 成绩最低%d名】\n", subName, n);
    for(i=count-1;i>=count-n;i--){
        float score;          
        if(subject==1){
            score=stuArr[i].score.cp;
        }else if(subject==2){
            score=stuArr[i].score.ldty;
        }else{
            score=stuArr[i].score.sxq;
        }
        printf("%s\t%s\t%s\t%s成绩：%.1f\n",
               stuArr[i].cla, stuArr[i].num, stuArr[i].name, subName, score);
    }
}

// 5. 删除/修改功能函数
//删除
int deleteStudent(Student stuArr[], int *count)
 {
    char cla[20], num[10];    //判断班级和学号
    printf("输入要删除的学生班级："); scanf("%s",cla);
    printf("输入要删除的学生学号："); scanf("%s",num);

    int i,index=-1;//index用来记录删除学生所在的位置，如果没找到该学生则index保持-1，报错
    for(i=0;i<*count;i++){
        if(strcmp(stuArr[i].cla,cla)==0&&strcmp(stuArr[i].num,num)==0){
            index=i;
            break;
        }
    }

    if (index==-1){
        printf("未找到该学生！\n");
        return 0;
    }

    for(i=index;i<*count-1;i++) stuArr[i]=stuArr[i+1];  
    (*count)--;//index后的元素全部往前移一位
    
    saveToFile(stuArr,*count);
    printf("已删除该学生，数据已更新到score.txt！\n");
    return 1;
}
//修改
int modifyStudent(Student stuArr[], int count) 
{
    char cla[20], num[10];
    printf("输入要修改的学生班级："); scanf("%s", cla);
    printf("输入要修改的学生学号："); scanf("%s", num);

    int i,index=-1;
    for(i=0;i<count;i++){
        if(strcmp(stuArr[i].cla,cla)==0&&strcmp(stuArr[i].num,num)==0){
            index=i;
            break;
        }
    }

    if(index==-1){
        printf("未找到该学生！\n");
        return 0;
    }

    printf("重新录入该学生成绩：\n");
    inputScore(&stuArr[index]);
    sortGrade(&stuArr[index]);
    saveToFile(stuArr, count);
    printf("已修改该学生成绩，数据已更新到score.txt！\n");
    return 1;
}

// 6. 批量操作功能函数
//增加
void batchAddStudent(Student stuArr[], int *count)
 {
    int num,i;
    printf("输入批量追加的学生数量：");
    scanf("%d",&num);
    if (num<=0||(*count+num)>MAX_STUDENT){
        printf("数量无效（超出上限/负数）！\n");
        return;
    }

    for (i=0;i<num;i++){
        printf("录入第%d个学生：\n",i+1);
        inputStudent(&stuArr[*count]);
        (*count)++;
    }
    // 保存批量追加的数据
    saveToFile(stuArr, *count);
    printf("批量追加%d个学生完成，数据已保存！\n", num);
}
//批量修改指定学生
void batchModifyStudent(Student stuArr[], int count) 
{
    int n, i;
    printf("输入要批量修改的学生数量：");
    scanf("%d", &n);
    if(n<=0 || n>count){
        printf("数量无效（负数/超出当前学生总数）！\n");
        return;
    }
    int modifyCount = 0;
    for(i=0; i<n; i++){
        printf("\n第%d个修改学生：\n", i+1);
        int res = modifyStudent(stuArr, count);
        if(res == 1){
            modifyCount++;
        }
    }
    printf("\n批量修改完成，共成功修改 %d 名学生！\n", modifyCount);
}
//批量删除指定学生
void batchDeleteStudent(Student stuArr[], int *count) 
{
    int n, i;
    printf("输入要批量删除的学生数量：");
    scanf("%d", &n);
    if(n<=0 || n>*count){
        printf("数量无效（负数/超出当前学生总数）！\n");
        return;
    }
    int deleteCount = 0;
    for(i=0; i<n; i++){
        printf("\n第%d个删除学生：\n", i+1);
        int res = deleteStudent(stuArr, &stuCount);
        if(res == 1){
            deleteCount++;
        }
    }
    printf("\n批量删除完成，共成功删除 %d 名学生！\n", deleteCount);
}