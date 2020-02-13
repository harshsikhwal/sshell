/*
 * MIT License
 *
 * Copyright (c) 2020 Harsh Sikhwal
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include<stdio.h>

int print_prompt_and_statement(char *statement)
{
    char *pwd;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        //printf("Working dir: %s", cwd);
        pwd = (char *)malloc(sizeof(cwd) + 2);
        strcpy(pwd, cwd);
        //printf("Working dir: %s", pwd);
        strcat(pwd, ">");
        //printf("Working dir: %s", pwd);
        printf("%s%s", pwd, statement);
        //free(&pwd);
        return 1;
    }
    else
    {
        return -1;
    }
}


int prompt()
{
    char *pwd;
    LOG("%s", "prompt>");
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        //printf("Working dir: %s", cwd);
        pwd = (char *)malloc(sizeof(cwd) + 2);
        strcpy(pwd, cwd);
        //printf("Working dir: %s", pwd);
        strcat(pwd, ">");
        //printf("Working dir: %s", pwd);
        printf("%s", pwd);
        //free(&pwd);
        return 1;
    }
    else
    {
        return -1;
    }
}
