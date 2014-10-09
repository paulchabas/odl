#ifndef FLEX_HEADER_OdlGenLexer_h
#define FLEX_HEADER_OdlGenLexer_h
#define YY_OdlScanner_CHAR char
#line 1 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\flexskel.h"
/* A lexical scanner header generated by flex */
/* MODIFIED FOR C++ CLASS BY Alain Coetmeur: coetmeur(at)icdc.fr */
/* Note that (at) mean the 'at' symbol that I cannot write */
/* because it is expanded to the class name */
/* made at Informatique-CDC, Research&development department */
/* company from the Caisse Des Depots et Consignations */


/*********************************************/
/* SYSTEM dependent declaration, includes... */
/*********************************************/
/* cfront 1.2 defines "c_plusplus" instead of "__cplusplus" */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
#ifndef YY_USE_PROTOS
#define YY_USE_PROTOS
#endif
#ifndef YY_USE_CLASS
#define YY_USE_CLASS
#endif
#else	/* ! __cplusplus */
#ifdef __STDC__
#ifdef __GNUC__
#else
#endif	/* __GNUC__ */
#ifndef YY_USE_PROTOS
#define YY_USE_PROTOS
#endif
#endif	/* __STDC__ */
#endif	/* ! __cplusplus */
/*********************************************/
/* COMPILER DEPENDENT   MACROS               */
/*********************************************/
/* use prototypes in function declarations */
#ifndef YY_PROTO
#ifdef YY_USE_PROTOS
#define YY_PROTO(proto) proto
#else
#define YY_PROTO(proto) ()
#endif
#endif
#include <stdio.h>




/* % here is the declaration from section1 %header{  */ 
#line 2 "OdlLexer.l"
#define YY_OdlScanner_LEX_PARAM  YY_OdlParserBase_STYPE *val, YY_OdlParserBase_LTYPE *loc
#line 3 "OdlLexer.l"
#define YY_OdlScanner_MEMBERS  public: int line, column;
#line 4 "OdlLexer.l"
#define YY_OdlScanner_CONSTRUCTOR_INIT  : line(1), column(1)
#line 9 "OdlLexer.l"

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG

#include <iostream>
#include<sstream>
#include <cassert>
#include "OdlGenParser.h"
#line 30 "OdlLexer.l"
#line 52 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\flexskel.h"

#ifdef YY_USE_CLASS
#ifdef YY_OdlScanner_IOSTREAM
#include <iostream.h>
#define YY_OdlScanner_IFILE  istream
#define YY_OdlScanner_OFILE ostream 
#define YY_OdlScanner_ERRFILE cerr

#ifndef YY_OdlScanner_IFILE_DEFAULT
#define YY_OdlScanner_IFILE_DEFAULT &cin
#endif

#ifndef YY_OdlScanner_OFILE_DEFAULT
#define YY_OdlScanner_OFILE_DEFAULT &cout
#endif

#endif
#endif

#ifndef YY_OdlScanner_IFILE
#define YY_OdlScanner_IFILE FILE 
#endif

#ifndef YY_OdlScanner_OFILE
#define YY_OdlScanner_OFILE FILE 
#endif

#ifndef YY_OdlScanner_ERRFILE
#define YY_OdlScanner_ERRFILE stderr
#endif

#ifndef YY_OdlScanner_IFILE_DEFAULT
#define YY_OdlScanner_IFILE_DEFAULT stdin
#endif

#ifndef YY_OdlScanner_OFILE_DEFAULT
#define YY_OdlScanner_OFILE_DEFAULT stdout
#endif




#ifndef YY_OdlScanner_TEXT
#define YY_OdlScanner_TEXT yytext
#endif
#ifndef YY_OdlScanner_LENG
#define YY_OdlScanner_LENG yyleng
#endif
#ifndef YY_OdlScanner_IN
#define YY_OdlScanner_IN yyin
#endif
#ifndef YY_OdlScanner_OUT
#define YY_OdlScanner_OUT yyout
#endif

#ifndef YY_OdlScanner_LEX_RETURN
#define YY_OdlScanner_LEX_RETURN int
#else
#ifndef YY_OdlScanner_LEX_DEFINED
#define YY_OdlScanner_LEX_DEFINED 
#endif
#endif

#ifndef YY_OdlScanner_LEX
#define YY_OdlScanner_LEX yylex
#else
#ifndef YY_OdlScanner_LEX_DEFINED
#define YY_OdlScanner_LEX_DEFINED 
#endif
#endif

#ifndef YY_OdlScanner_LEX_PARAM
#ifndef YY_USE_PROTOS
#define YY_OdlScanner_LEX_PARAM 
#else
#define YY_OdlScanner_LEX_PARAM void
#endif
#else
#ifndef YY_OdlScanner_LEX_DEFINED
#define YY_OdlScanner_LEX_DEFINED 
#endif
#endif

#ifndef YY_OdlScanner_LEX_PARAM_DEF
#define YY_OdlScanner_LEX_PARAM_DEF
#else
#ifndef YY_OdlScanner_LEX_DEFINED
#define YY_OdlScanner_LEX_DEFINED 
#endif
#endif

#ifndef YY_OdlScanner_RESTART
#define YY_OdlScanner_RESTART yyrestart
#endif
#ifndef YY_OdlScanner_SWITCH_TO_BUFFER
#define YY_OdlScanner_SWITCH_TO_BUFFER yy_switch_to_buffer
#endif
#ifndef YY_OdlScanner_LOAD_BUFFER_STATE
#define YY_OdlScanner_LOAD_BUFFER_STATE yy_load_buffer_state
#endif

#ifndef YY_OdlScanner_CREATE_BUFFER
#define YY_OdlScanner_CREATE_BUFFER yy_create_buffer
#ifndef YY_USE_CLASS
#ifndef yy_new_buffer
#define yy_new_buffer yy_create_buffer
#endif
#endif
#endif
#ifndef YY_OdlScanner_DELETE_BUFFER
#define YY_OdlScanner_DELETE_BUFFER yy_delete_buffer
#endif
#ifndef YY_OdlScanner_INIT_BUFFER
#define YY_OdlScanner_INIT_BUFFER yy_init_buffer
#endif



#ifdef YY_OdlScanner_FLEX_DEBUG
#ifndef YY_OdlScanner_DEBUG
#define YY_OdlScanner_DEBUG 1
#endif
#else
#ifndef YY_OdlScanner_DEBUG
#define YY_OdlScanner_DEBUG 0
#endif
#endif

#if YY_OdlScanner_DEBUG != 0
#ifndef YY_OdlScanner_DEBUG_FLAG
#define YY_OdlScanner_DEBUG_FLAG yy_flex_debug
#endif
#ifndef YY_OdlScanner_DEBUG_INIT
#define YY_OdlScanner_DEBUG_INIT 1
#endif
#endif




#ifndef YY_USE_CLASS
#ifndef YY_OdlScanner_CURRENT_BUFFER
#define YY_OdlScanner_CURRENT_BUFFER yy_current_buffer
#endif
typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern void YY_OdlScanner_RESTART YY_PROTO(( YY_OdlScanner_IFILE *input_file ));
extern void YY_OdlScanner_SWITCH_TO_BUFFER YY_PROTO(( YY_BUFFER_STATE new_buffer ));
extern void YY_OdlScanner_LOAD_BUFFER_STATE YY_PROTO(( void ));
extern YY_BUFFER_STATE YY_OdlScanner_CREATE_BUFFER YY_PROTO(( YY_OdlScanner_IFILE *file, int size ));
extern void YY_OdlScanner_DELETE_BUFFER YY_PROTO(( YY_BUFFER_STATE b ));
extern void YY_OdlScanner_INIT_BUFFER YY_PROTO(( YY_BUFFER_STATE b, YY_OdlScanner_IFILE *file ));

#if YY_OdlScanner_DEBUG != 0
extern int YY_OdlScanner_DEBUG_FLAG ;
#endif
extern YY_OdlScanner_CHAR  *YY_OdlScanner_TEXT;
extern int YY_OdlScanner_LENG;
extern YY_OdlScanner_IFILE *YY_OdlScanner_IN;
extern YY_OdlScanner_OFILE *YY_OdlScanner_OUT;
#ifdef YY_OdlScanner_LEX_DEFINED
extern YY_OdlScanner_LEX_RETURN YY_OdlScanner_LEX ( YY_OdlScanner_LEX_PARAM )
YY_OdlScanner_LEX_PARAM_DEF
#else
#ifndef YY_DECL
extern YY_OdlScanner_LEX_RETURN YY_OdlScanner_LEX ( YY_OdlScanner_LEX_PARAM )
YY_OdlScanner_LEX_PARAM_DEF
#else
/* no declaration if oldstyle flex */
#endif
#endif
#else

#ifndef YY_OdlScanner_CURRENT_BUFFER
#define YY_OdlScanner_CURRENT_BUFFER YY_CURRENT_BUFFER
#endif
#ifndef YY_OdlScanner_CLASS
#define YY_OdlScanner_CLASS OdlScanner
#endif
#ifndef YY_OdlScanner_ECHO
#define YY_OdlScanner_ECHO yy_echo
#endif
#ifdef YY_OdlScanner_ECHO_PURE
#define YY_OdlScanner_ECHO_NOCODE
#endif

#ifndef YY_OdlScanner_ECHO_CODE
#ifndef YY_OdlScanner_IOSTREAM
#define YY_OdlScanner_ECHO_CODE fwrite( (char *) YY_OdlScanner_TEXT, YY_OdlScanner_LENG, 1, YY_OdlScanner_OUT );
#else
#define YY_OdlScanner_ECHO_CODE (YY_OdlScanner_OUT->write( (char *) YY_OdlScanner_TEXT, YY_OdlScanner_LENG));
#endif
#endif

#ifndef YY_OdlScanner_INPUT
#define YY_OdlScanner_INPUT yy_input
#endif
#ifdef YY_OdlScanner_INPUT_PURE
#define YY_OdlScanner_INPUT_NOCODE
#endif

#ifndef YY_OdlScanner_INPUT_CODE
#ifndef YY_OdlScanner_IOSTREAM
#define YY_OdlScanner_INPUT_CODE return result= fread(  buffer, 1,max_size,YY_OdlScanner_IN );
#else
#define YY_OdlScanner_INPUT_CODE if(YY_OdlScanner_IN->eof())  result=0;else {YY_OdlScanner_IN->read(buffer,max_size);result=YY_OdlScanner_IN->gcount();YY_OdlScanner_IN->clear(YY_OdlScanner_IN->rdstate()&(~ios::failbit));if(YY_OdlScanner_IN->bad()) result= -1;} return result;
#endif
#endif

#ifdef YY_OdlScanner_FATAL_ERROR_PURE
#define YY_OdlScanner_FATAL_ERRO_NOCODE
#endif
#ifndef YY_OdlScanner_FATAL_ERROR
#define YY_OdlScanner_FATAL_ERROR yy_fatal_error
#endif

#ifndef YY_OdlScanner_FATAL_ERROR_CODE
#ifndef YY_OdlScanner_IOSTREAM
#define YY_OdlScanner_FATAL_ERROR_CODE fputs( msg, YY_OdlScanner_ERRFILE );putc( '\n', YY_OdlScanner_ERRFILE );exit( 1 );
#else
#define YY_OdlScanner_FATAL_ERROR_CODE YY_OdlScanner_ERRFILE<< msg <<endl;exit( 1 );
#endif
#endif

#ifndef YY_OdlScanner_WRAP
#define YY_OdlScanner_WRAP yy_wrap
#endif
#ifdef YY_OdlScanner_WRAP_PURE
#define YY_OdlScanner_WRAP_NOCODE
#endif
#ifndef YY_OdlScanner_WRAP_CODE
#define YY_OdlScanner_WRAP_CODE return 1;
#endif


#ifndef YY_OdlScanner_INHERIT
#define YY_OdlScanner_INHERIT
#endif
#ifndef YY_OdlScanner_MEMBERS
#define YY_OdlScanner_MEMBERS 
#endif
#ifndef YY_OdlScanner_CONSTRUCTOR_PARAM
#define YY_OdlScanner_CONSTRUCTOR_PARAM
#endif
#ifndef YY_OdlScanner_CONSTRUCTOR_CODE
#define YY_OdlScanner_CONSTRUCTOR_CODE
#endif
#ifndef YY_OdlScanner_CONSTRUCTOR_INIT
#define YY_OdlScanner_CONSTRUCTOR_INIT
#endif
#ifndef YY_OdlScanner_DESTRUCTOR_CODE
#define YY_OdlScanner_DESTRUCTOR_CODE
#endif
typedef struct yy_buffer_state *YY_BUFFER_STATE;

class YY_OdlScanner_CLASS YY_OdlScanner_INHERIT
{
 private:/* data */
 YY_OdlScanner_CHAR  *yy_c_buf_p;
 YY_OdlScanner_CHAR  yy_hold_char;
 int yy_n_chars;
 int yy_init;
 int yy_start;
 int yy_did_buffer_switch_on_eof;
 private: /* functions */
 void yy_initialize();
 int input();
 int yyinput() {return input();};
 int yy_get_next_buffer();
 void yyunput( YY_OdlScanner_CHAR  c, YY_OdlScanner_CHAR  *buf_ptr );
 /* use long instead of yy_state_type because it is undef */
 long yy_get_previous_state_ ( void );
 long yy_try_NUL_trans_  ( long current_state_ );
 protected:/* non virtual */
 YY_BUFFER_STATE YY_OdlScanner_CURRENT_BUFFER;
 void YY_OdlScanner_RESTART ( YY_OdlScanner_IFILE *input_file );
 void YY_OdlScanner_SWITCH_TO_BUFFER( YY_BUFFER_STATE new_buffer );
 void YY_OdlScanner_LOAD_BUFFER_STATE( void );
 YY_BUFFER_STATE YY_OdlScanner_CREATE_BUFFER( YY_OdlScanner_IFILE *file, int size );
 void YY_OdlScanner_DELETE_BUFFER( YY_BUFFER_STATE b );
 void YY_OdlScanner_INIT_BUFFER( YY_BUFFER_STATE b, YY_OdlScanner_IFILE *file );
 protected: /* virtual */
 virtual void YY_OdlScanner_ECHO()
#ifdef YY_OdlScanner_ECHO_PURE
  =0
#endif
  ;
 virtual int  YY_OdlScanner_INPUT(char  *buf,int &result,int max_size)
#ifdef YY_OdlScanner_INPUT_PURE
  =0
#endif
  ;
 virtual void  YY_OdlScanner_FATAL_ERROR(char *msg)
#ifdef YY_OdlScanner_FATAL_ERROR_PURE
  =0
#endif
  ;
 virtual int  YY_OdlScanner_WRAP()
#ifdef YY_OdlScanner_WRAP_PURE
  =0
#endif
  ;
 public:
 YY_OdlScanner_CHAR  *YY_OdlScanner_TEXT;
 int YY_OdlScanner_LENG;
 YY_OdlScanner_IFILE *YY_OdlScanner_IN;
 YY_OdlScanner_OFILE *YY_OdlScanner_OUT;
 YY_OdlScanner_LEX_RETURN YY_OdlScanner_LEX ( YY_OdlScanner_LEX_PARAM);
 YY_OdlScanner_CLASS(YY_OdlScanner_CONSTRUCTOR_PARAM) ;
 virtual ~YY_OdlScanner_CLASS() ;
#if YY_OdlScanner_DEBUG != 0
 int YY_OdlScanner_DEBUG_FLAG;
#endif
 public: /* added members */
 YY_OdlScanner_MEMBERS 
};
#endif



/* declaration of externs for public use of yylex scanner */

/* % here is the declaration from section2 %header{ */ 
#line 167 "OdlLexer.l"
#endif
#line 375 "D:\\GitHub\\odl\\extern\\flexppbisonpp\\flexskel.h"

/* end of generated header */
