int registerOnBeginTest(onBegin func);
int registerOnNumberTest(onNumber func);
int registerOnStringTest(onString func);
int registerOnEndTest(onEnd func);

int testParseOnBegin(onBegin func, const char* text);
int testParseOnString(onString func, const char* text);
int testParseOnNumber(onNumber func, const char* text);
int testParseOnEnd(onEnd func, const char* text);

int testParse(const char* text);