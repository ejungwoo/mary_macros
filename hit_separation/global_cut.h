#define CQ1   200.
#define CQ2   3400.

#define CNPD  30

#define CY1  -100.
#define CY2  -200.
#define CY3  -250.
#define CY4  -500.

#define CLY1  20
#define CLY2  80

#define CRW1  10
#define CRW2  40
#define CRW3  62
#define CRW4  98

TString gvname = "ps3";

//TCut cut_total = Form("q1>%f&&q1<%f && npad1>%d && ((y<%f&&y>%f)||(y<%f)) && layer>=%d&&layer<%d && ((row>=%d&&row<%d)||(row>%d&&row<%d))",CQ1,CQ2,CNPD,CY1,CY2,CY3,CLY1,CLY2,CRW1,CRW2,CRW3,CRW4);
  TCut cut_total = Form("q1>%f&&q1<%f && npad1>%d && y<%f                   && layer>=%d&&layer<%d && ((row>=%d&&row<%d)||(row>%d&&row<%d))",CQ1,CQ2,CNPD,CY1,CLY1,CLY2,CRW1,CRW2,CRW3,CRW4);
  TCut cut_found = cut_total + TCut("found");
  
  TCut cut_2 = Form("q1>%f&&q1<%f && npad1>%d && ((y<%f&&y>%f)||(y<%f)) && layer>=%d&&layer<%d && ((row>=%d&&row<%d)||(row>%d&&row<%d))",CQ1,CQ2,CNPD,CY1,CY2,CY3,CLY1,CLY2,CRW1,CRW2,CRW3,CRW4);
  TCut cut_2hit2 = Form("q2>%f&&q1<%f && npad2>%d && ((y-dtrue<%f&&y-dtrue>%f)||(y-dtrue<%f&&y-dtrue>%f))",CQ1,CQ2,CNPD,CY1,CY2,CY3,CY4);
  TCut cut_2hit   = cut_2 + cut_2hit2;
