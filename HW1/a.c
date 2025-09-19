#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

bool isLoggedIn = false; // trạng thái đăng nhập {true: đang đăng nhập, false: chưa đăng nhập}
char userName[5000];     // lưu userName đang đăng nhập

int logAction;      // Chức năng lựa chọn {1, 2, 3, 4}
char logInput[500]; // Giá trị người dùng cung cấp
char logResult[10]; // Mã kết quả

bool checkUser(char *userName, FILE *f);
void login(FILE *f);
void postMessage();
void logout();
void exitApp();
void appendLog(FILE *f1);

int main(int argc, char *argv[])
{

    // Kết nối dữ liệu
    FILE *f = fopen("account.txt", "r");
    if (f == NULL)
    {
        printf("Failed to connect db !!!!\n");
        return 1;
    }

    FILE *f1 = fopen("log.txt", "a");
    if (f1 == NULL)
    {
        printf("Failed to connect LOG file !!!!\n");
        return 1;
    }

    char choice[100];

    while (1)
    {

        // Menu chương trình
        printf("----Menu------\n");
        printf("1. Log in\n");
        printf("2. Post message\n");
        printf("3. Logout\n");
        printf("4. Exit\n");
        printf("----Menu------\n");
        printf("Enter your choice: ");
        scanf("%s", choice);
        getchar(); // xóa '\n' thừa trong stdin

        // Xóa log cũ
        logAction = 0;
        strcpy(logInput, "");
        strcpy(logResult, "");

        if (strcmp(choice, "1") == 0)
        {
            login(f);
            appendLog(f1);
        }
        else if (strcmp(choice, "2") == 0)
        {
            postMessage();
            appendLog(f1);
        }
        else if (strcmp(choice, "3") == 0)
        {
            logout();
            appendLog(f1);
        }
        else if (strcmp(choice, "4") == 0)
        {
            exitApp();
            appendLog(f1);
        }
        else
        {
            printf("Invalid choice !!!!\n");
        }
    }

    fclose(f1);
    fclose(f);

    return 0;
}

bool checkUser(char *userName, FILE *f)
{
    char line[500];

    rewind(f); // trỏ f về đầu file

    while (fgets(line, sizeof(line), f) != NULL)
    {
        char user[500];
        int status;
        sscanf(line, "%s %d", user, &status);

        if (strcmp(userName, user) == 0)
        {
            if (status == 0)
            {
                printf("Account is banned\n");
                return false;
            }
            return true;
        }
    }
    printf("Account is not exist\n");
    return false;
}

void login(FILE *f)
{
    logAction = 1;

    if (isLoggedIn)
    {
        printf("You have already logged in\n");
        strcpy(logInput, userName);
        strcpy(logResult, "-ERR");
    }
    else
    {
        printf("Enter userName: ");
        scanf("%s", userName);

        strcpy(logInput, userName);
        if (checkUser(userName, f))
        {
            printf("Hello %s\n", userName);
            isLoggedIn = true;
            strcpy(logResult, "+OK");
        }
        else
        {
            strcpy(logResult, "-ERR");
        }
    }
}

void postMessage()
{
    logAction = 2;
    char message[500];
    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; // xóa \n cuối message
    strcpy(logInput, message);
    if (isLoggedIn)
    {
        printf("Successful post\n");
        strcpy(logResult, "+OK");
    }
    else
    {
        printf("You have not logged in.\n");
        strcpy(logResult, "-ERR");
    }
}
void logout()
{
    logAction = 3;
    strcpy(logInput, "");
    if (isLoggedIn)
    {
        isLoggedIn = false;
        strcpy(userName, "");
        strcpy(logResult, "+OK");
        printf("Successful log out\n");
    }
    else
    {
        strcpy(logResult, "-ERR");
        printf("You have not logged in.\n");
    }
}

void exitApp()
{
    logAction = 4;
    strcpy(logInput, "");
    strcpy(logResult, "+OK");
}

void appendLog(FILE *f1)
{
    // Lấy thời gian hiện tại
    time_t rawtime;
    struct tm *timeinfo;
    char logTimestamp[20]; // mảng lưu kết quả format

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(logTimestamp, sizeof(logTimestamp), "%d/%m/%Y %H:%M:%S", timeinfo);

    // Ghi log vào file theo format: [dd/mm/yyyy hh:mm:ss] $ logAction $ logInput $ logResult
    fprintf(f1, "[%s] $ %d $ %s $ %s\n", logTimestamp, logAction, logInput, logResult);
    fflush(f1); // ghi luôn ra file log
}
