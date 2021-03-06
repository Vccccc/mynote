## 可变长数组 char data[0]
在结构体最后放入char data[0]，可以让数组长度是可变的。
```c
struct buffer
{
  int data_len; // 长度
  char data[0]; // 起始地址
}
```

#### 测试
可以观察到char data[0]和char data[]都不占空间，而char* data，要占一个指针大小的空间。data[0]和data[]的地址是紧接着结构体的地址，而char* data的地址不是。
```c
 1 #include <stdio.h>
 2 #include <stdlib.h>
 3 #include <string.h>
 4 #include <stdint.h>
 5 
 6 typedef struct
 7 {
 8     int data_len;
 9     char data[0];
10 }buff_st_1;
11 
12 typedef struct
13 {
14     int data_len;
15     char *data;
16 }buff_st_2;
17 
18 typedef struct 
19 {
20     int data_len;
21     char data[];
22 }buff_st_3;
23 
24 int main()
25 {
26     printf("sizeof(buff_st_1)=%u\n", sizeof(buff_st_1));
27     printf("sizeof(buff_st_2)=%u\n", sizeof(buff_st_2));
28     printf("sizeof(buff_st_3)=%u\n", sizeof(buff_st_3));
29 
30     buff_st_1 buff1;
31     buff_st_2 buff2;
32     buff_st_3 buff3;
33 
34     printf("buff1 address:%p,buff1.data_len address:%p,buff1.data address:%p\n",
35         &buff1, &(buff1.data_len), buff1.data);
36 
37     printf("buff2 address:%p,buff2.data_len address:%p,buff2.data address:%p\n",
38         &buff2, &(buff2.data_len), buff2.data);
39 
40     printf("buff3 address:%p,buff3.data_len address:%p,buff3.data address:%p\n",
41         &buff3, &(buff3.data_len), buff3.data);
42 
43     return 0;
44 }

output:
sizeof(buff_st_1)=4
sizeof(buff_st_2)=16
sizeof(buff_st_3)=4
buff1 address:0x7fff96e59c40,buff1.data_len address:0x7fff96e59c40,buff1.data 
address:0x7fff96e59c44
buff2 address:0x7fff96e59c30,buff2.data_len address:0x7fff96e59c30,buff2.data 
address:(nil)
buff3 address:0x7fff96e59c50,buff3.data_len address:0x7fff96e59c50,buff3.data 
address:0x7fff96e59c54
```


#### 测试2
采用char *data，需要进行二次分配，操作比较麻烦，很容易造成内存泄漏。而直接采用变长的数组，只需要分配一次，然后进行取值即可以。
```c
#include <stdio.h>
 2 #include <stdlib.h>
 3 #include <string.h>
 4 #include <stdint.h>
 5 
 6 typedef struct
 7 {
 8     int data_len;
 9     char data[0];
10 }buff_st_1;
11 
12 typedef struct
13 {
14     int data_len;
15     char *data;
16 }buff_st_2;
17 
18 typedef struct 
19 {
20     int data_len;
21     char data[];
22 }buff_st_3;
23 
24 typedef struct 
25 {
26     uint32_t id;
27     uint32_t age;
28 }student_st;
29 
30 
31 void print_stu(const student_st *stu)
32 {
33     printf("id:%u,age:%u\n", stu->id, stu->age);
34 }
35 
36 int main()
37 {
38     student_st *stu = (student_st *)malloc(sizeof(student_st));
39     stu->id = 100;
40     stu->age = 23;
41 
42     student_st *tmp = NULL;
43 
44     buff_st_1 *buff1 = (buff_st_1 *)malloc(sizeof(buff_st_1) + sizeof(student_st));
45     buff1->data_len = sizeof(student_st);
46     memcpy(buff1->data, stu, buff1->data_len);
47     printf("buff1 address:%p,buff1->data_len address:%p,buff1->data address:%p\n",
48         buff1, &(buff1->data_len), buff1->data);
49 
50     tmp = (student_st*)buff1->data;
51     print_stu(tmp);
52 
53     buff_st_2 *buff2 = (buff_st_2 *)malloc(sizeof(buff_st_2));
54     buff2->data_len = sizeof(student_st);
55     buff2->data = (char *)malloc(buff2->data_len);
56     memcpy(buff2->data, stu, buff2->data_len);
57     printf("buff2 address:%p,buff2->data_len address:%p,buff2->data address:%p\n",
58         buff2, &(buff2->data_len), buff2->data);
59 
60     tmp = (student_st *)buff2->data;
61     print_stu(tmp);
62 
63     buff_st_3 *buff3 = (buff_st_3 *)malloc(sizeof(buff_st_3) + sizeof(student_st));
64     buff3->data_len = sizeof(student_st);
65     memcpy(buff3->data, stu, buff3->data_len);
66     printf("buff3 address:%p,buff3->data_len address:%p,buff3->data address:%p\n",
67         buff3, &(buff3->data_len), buff3->data);
68 
69     tmp = (student_st*)buff1->data;
70     print_stu(tmp);
71 
72     free(buff1);
73 
74     free(buff2->data);
75     free(buff2);
76 
77     free(buff3);
78     free(stu);
79     return 0;
80 }

output:
buff1 address:0xfc2030,buff1->data_len address:0xfc2030,buff1->data address:0xfc2034
id:100,age:23
buff2 address:0xfc2460,buff2->data_len address:0xfc2460,buff2->data address:0xfc2480
id:100,age:23
buff3 address:0xfc24a0,buff3->data_len address:0xfc24a0,buff3->data address:0xfc24a4
id:100,age:23
```

