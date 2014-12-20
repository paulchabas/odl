..\\..\\extern\\flexppbisonpp\bisonpp.exe -d -oOdlGenParser.cpp -hOdlGenParser.h OdlParser.y

rem "copy of OdlParser.y in vcprojet directory to make file/line debug working :'("
copy OdlParser.y ..\\..\\VS2013\\vodlparserlexer\\OdlParser.y
pause