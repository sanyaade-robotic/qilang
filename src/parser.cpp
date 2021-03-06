/*
** Author(s):
**  - Cedric GESTES <gestes@aldebaran-robotics.com>
**
** Copyright (C) 2014 Aldebaran Robotics
*/

#include <qi/os.hpp>
#include <qilang/parser.hpp>
#include <qilang/node.hpp>
#include "parser_p.hpp"
#include <iostream>
#include <fstream>
#include "grammar.tab.hpp"

int  qilang_lex_init(void**);
int  qilang_lex_destroy(void*);
void qilang_set_extra(qilang::Parser*, void *);
struct yyscan_t;
void qilang_set_debug(int debug_flag, void* yyscanner);

qiLogCategory("qilang.parser");

namespace qilang {

  Parser::Parser(const FileReaderPtr &file)
    : file(file)
    , _result(newParseResult())
    , _parsed(false)
    , parser(this)
    , linesSinceLastComment(0)
  {
    _result->filename = file->filename();
    qilang_lex_init(&scanner);
    qilang_set_extra(this, scanner);
  }

  Parser::~Parser()
  {
    qilang_lex_destroy(scanner);
  }

  void Parser::parse() {
    if (_parsed)
      return;
    _parsed = true;
    loc.initialize(const_cast<std::string*>(&file->filename()));
    std::string pdebug = qi::os::getenv("QILANG_PARSER_DEBUG");
    if (!pdebug.empty() && pdebug != "0") {
      parser.set_debug_level(1);
    }
    else {
      parser.set_debug_level(0);
    }
    std::string ldebug = qi::os::getenv("QILANG_LEXER_DEBUG");
    if (!ldebug.empty() && ldebug != "0") {
      qilang_set_debug(1, scanner);
    }
    else {
      qilang_set_debug(0, scanner);
    }
    try {
      parser.parse();
    } catch (const ParseException& pe) {
      _result->ast.clear();
      _result->addDiag(Diagnostic(DiagnosticType_Error, pe.what(), pe.loc()));
    }
  }

  void Diagnostic::print(std::ostream &out) const {
    out << loc() << ":";

    switch (type()) {
      case DiagnosticType_Error:
        out << "error: ";
        break;
      case DiagnosticType_Warning:
        out << "warning: ";
        break;
      case DiagnosticType_Info:
        out << "info: ";
        break;
      default:
        break;
    }

    out << what() << std::endl;
    out << qilang::getErrorLine(filename(), loc());
  }

  void ParseResult::printMessage(std::ostream &out) const {
    for (unsigned i = 0; i < _messages.size(); ++i)
      _messages.at(i).print(out);
  }

  ParseResultPtr Parser::result() {
    parse();
    return _result;
  }

  Location makeLocation(const yy::location& loc) {
    if (loc.begin.filename)
      return Location(loc.begin.line, loc.begin.column, loc.end.line, loc.end.column, *loc.begin.filename);
    else {
      qiLogWarning() << "missing filename for location";
      return Location(loc.begin.line, loc.begin.column, loc.end.line, loc.end.column);
    }
  }

  std::string getErrorLine(const std::string& filename, const Location& loc) {
    std::ifstream is;
    std::string   ret;

    //no location provided just drop
    if (loc.beg_column == 0 || loc.beg_line == 0)
      return std::string();
    is.open(filename.c_str());
    if (!is.is_open())
      return std::string();

    std::string ln;
    unsigned int lico = loc.beg_line;
    for (unsigned int i = 0; i < lico; ++i) {
      if (!is.good())
        return std::string();
      getline(is, ln);
    }
    //ret = "";
    ret = ln + "\n";
    unsigned int cbeg = loc.beg_column;
    unsigned int cend = loc.end_column;
    int count = cend - cbeg;
    int space = cbeg - 1;
    //multiline error just display the beginning
    if (loc.end_line != loc.beg_line)
      count = 1;
    space = space < 0 ? 0 : space;
    count = count < 1 ? 1 : count;
    for (int i = 0; i < space; ++i)
      ret += " ";
    for (int i = 0; i < count; ++i)
      ret += "^";
    ret += "\n";
    return ret;
  }

  //public interface
  ParseResultPtr parse(const FileReaderPtr& file) {
    ParseResultPtr ret = newParseResult();
    ret->filename = file->filename();
    if (!file->isOpen()) {
      ret->addDiag(Diagnostic(DiagnosticType_Error, "Can't open file '" + file->filename() + "'"));
      return ret;
    }
    Parser p(file);
    return p.result();
  }

}
