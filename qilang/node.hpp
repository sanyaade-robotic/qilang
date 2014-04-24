/*
** Author(s):
**  - Cedric GESTES <gestes@aldebaran-robotics.com>
**
** Copyright (C) 2014 Aldebaran Robotics
*/

#ifndef QILANG_NODE_HPP
#define QILANG_NODE_HPP

#include <qilang/api.hpp>

#include <string>
#include <vector>
#include <stdexcept>
#include <qi/types.hpp>
#include <boost/shared_ptr.hpp>
#include <qitype/anyvalue.hpp>


namespace qilang {


class Node;

// STMT: Package Management
class StmtNode;
class PackageNode;
class ImportNode;
class ObjectDefNode;
class PropertyDefNode;
class AtNode;
class VarDefNode;

// EXPR: Const Data
class ConstDataNode;  // VIRTUAL
class BoolConstDataNode;
class IntConstDataNode;
class FloatConstDataNode;
class StringConstDataNode;
class ListConstDataNode;
class DictConstDataNode;
class TupleConstDataNode;

// EXPR: Type Expr
class TypeExprNode;        //VIRTUAL
class SimpleTypeExprNode;
class ListTypeExprNode;
class MapTypeExprNode;
class TupleTypeExprNode;

// EXPR
class ExprNode;        //VIRTUAL: dep on TypeExpr, ConstData
class BinaryOpExprNode;
class UnaryOpExprNode;
class VarExprNode;
class ConstDataExprNode;

// Interface Declaration
class DeclNode;          //VIRTUAL
class InterfaceDeclNode;
class FnDeclNode;
class EmitDeclNode;
class PropDeclNode;
class StructDeclNode; //Struct Decl
class FieldDeclNode;
class ConstDeclNode;


typedef std::vector<std::string>          StringVector;

typedef boost::shared_ptr<Node>           NodePtr;
typedef std::vector<NodePtr>              NodePtrVector;

typedef boost::shared_ptr<DeclNode>        DeclNodePtr;
typedef std::vector<DeclNodePtr>          DeclNodePtrVector;

typedef boost::shared_ptr<StmtNode>       StmtNodePtr;
typedef std::vector<StmtNodePtr>          StmtNodePtrVector;

typedef boost::shared_ptr<TypeExprNode>   TypeExprNodePtr;
typedef std::vector<TypeExprNodePtr>      TypeExprNodePtrVector;

typedef boost::shared_ptr<ExprNode>       ExprNodePtr;
typedef std::vector<ExprNodePtr>          ExprNodePtrVector;

typedef boost::shared_ptr<ConstDataNode>  ConstDataNodePtr;
typedef std::vector<ConstDataNodePtr>     ConstDataNodePtrVector;

typedef std::pair<ConstDataNodePtr, ConstDataNodePtr> ConstDataNodePtrPair;
typedef std::vector<ConstDataNodePtrPair>             ConstDataNodePtrPairVector;


/* All Statements
 */
class DeclNodeVisitor {
public:
  virtual void acceptDecl(const DeclNodePtr& node) = 0;

  // Interface Declaration
  virtual void visitDecl(InterfaceDeclNode* node) = 0;
  virtual void visitDecl(FnDeclNode* node) = 0;
  virtual void visitDecl(EmitDeclNode* node) = 0;
  virtual void visitDecl(PropDeclNode* node) = 0;

  // Struct Declaration
  virtual void visitDecl(StructDeclNode* node) = 0;
  virtual void visitDecl(ConstDeclNode* node) = 0;
  virtual void visitDecl(FieldDeclNode* node) = 0;
};

class StmtNodeVisitor {
public:
  virtual void acceptStmt(const StmtNodePtr& node) = 0;

  // Package Management
  virtual void visitStmt(PackageNode* node) = 0;
  virtual void visitStmt(ImportNode* node) = 0;

  // Object Definitions
  virtual void visitStmt(ObjectDefNode* node) = 0;
  virtual void visitStmt(PropertyDefNode* node) = 0;
  virtual void visitStmt(AtNode* node) = 0;

  // Definitions
  virtual void visitStmt(VarDefNode* node) = 0;
};


class ExprNodeVisitor {
public:
  virtual void acceptExpr(const ExprNodePtr& node) = 0;

  // Expr
  virtual void visitExpr(BinaryOpExprNode* node) = 0;
  virtual void visitExpr(UnaryOpExprNode* node) = 0;
  virtual void visitExpr(VarExprNode* node) = 0;
  virtual void visitExpr(ConstDataExprNode* node) = 0;
};

/** Const Data Expression Visitor
 */
class ConstDataNodeVisitor {
public:
  virtual void acceptData(const ConstDataNodePtr& node) = 0;

  virtual void visitData(BoolConstDataNode* node) = 0;
  virtual void visitData(IntConstDataNode* node) = 0;
  virtual void visitData(FloatConstDataNode* node) = 0;
  virtual void visitData(StringConstDataNode* node) = 0;
  virtual void visitData(TupleConstDataNode* node) = 0;
  virtual void visitData(ListConstDataNode* node) = 0;
  virtual void visitData(DictConstDataNode* node) = 0;
};

/** Type Expression Visitor
 */
class TypeExprNodeVisitor {
protected:
public:
  virtual void acceptTypeExpr(const TypeExprNodePtr& node) = 0;

  virtual void visitTypeExpr(SimpleTypeExprNode* node) = 0;
  virtual void visitTypeExpr(ListTypeExprNode* node) = 0;
  virtual void visitTypeExpr(MapTypeExprNode* node) = 0;
  virtual void visitTypeExpr(TupleTypeExprNode* node) = 0;
};


enum NodeKind {
  NodeKind_Expr,
  NodeKind_TypeExpr,
  NodeKind_ConstData,
  NodeKind_Decl,
  NodeKind_Stmt
};

enum NodeType {
  NodeType_Package,
  NodeType_Import,

  NodeType_BinOpExpr,
  NodeType_UOpExpr,
  NodeType_VarExpr,
  NodeType_ConstDataExpr,

  NodeType_SimpleTypeExpr,
  NodeType_MapTypeExpr,
  NodeType_ListTypeExpr,
  NodeType_TupleTypeExpr,

  NodeType_BoolData,
  NodeType_IntData,
  NodeType_FloatData,
  NodeType_StringData,
  NodeType_MapData,
  NodeType_ListData,
  NodeType_TupleData,

  NodeType_ObjectDef,
  NodeType_PropDef,
  NodeType_VarDef,
  NodeType_At,

  NodeType_InterfaceDecl,
  NodeType_FnDecl,
  NodeType_EmitDecl,
  NodeType_PropDecl,

  NodeType_StructDecl,
  NodeType_FieldDecl,
  NodeType_ConstDecl
};

//Base Node used to describe the AST
class QILANG_API Node
{
public:
  Node(NodeKind kind, NodeType type);
  virtual ~Node() {}

  NodeKind kind() const { return _kind; }
  NodeType type() const { return _type; }

private:
  NodeKind _kind;
  NodeType _type;
};



#if 0
class Node2;
typedef std::map<std::string, qi::AnyValue> AttributeMap;
typedef boost::shared_ptr<Node2> Node2Ptr;
typedef std::vector<Node2Ptr> NodeVector;

class QILANG_API Node2 {
public:
  explicit Node2(NodeType type, const AttributeMap& map, const NodeVector& children);
  explicit Node2(NodeType type, const AttributeMap& map);
  explicit Node2(NodeType type);

  Node2& setAttr(const std::string& name, qi::AutoAnyReference value);
  qi::AnyValue attr(const std::string& name);

  Node2& addChild(const Node2& node);

  NodeVector children() { return _children; }

  NodeKind     _kind;
  NodeType     _type;
  AttributeMap _attributes;
  NodeVector   _children;
};

class DictConstDataNode2: public Node2 {
  DictConstDataNode2(ConstDataNodePtrPairVector datas):
    Node2(NodeKind_ConstData)
  {
    setAttr("data", datas);
  }
};
#endif



enum UnaryOpCode {
  UnaryOpCode_Negate,
  UnaryOpCode_Minus
};

enum BinaryOpCode {
  BinaryOpCode_BoolOr,
  BinaryOpCode_BoolAnd,

  BinaryOpCode_And,
  BinaryOpCode_Or,
  BinaryOpCode_Xor,

  BinaryOpCode_EqEq,
  BinaryOpCode_Ne,
  BinaryOpCode_Gt,
  BinaryOpCode_Lt,
  BinaryOpCode_Ge,
  BinaryOpCode_Le,

  BinaryOpCode_Plus,
  BinaryOpCode_Minus,
  BinaryOpCode_Divide,
  BinaryOpCode_Multiply,
  BinaryOpCode_Modulus,

  BinaryOpCode_FetchArray, // a[b]
};

QILANG_API const std::string &UnaryOpCodeToString(UnaryOpCode op);
QILANG_API const std::string &BinaryOpCodeToString(BinaryOpCode op);

// ####################
// # EXPR Node
// ####################

class QILANG_API ExprNode : public Node {
protected:
  explicit ExprNode(NodeKind kind, NodeType type)
    : Node(kind, type)
  {}
  explicit ExprNode(NodeType type)
    : Node(NodeKind_Expr, type)
  {}
public:
  virtual void accept(ExprNodeVisitor *visitor) = 0;
};

class QILANG_API BinaryOpExprNode : public ExprNode {
public:
  BinaryOpExprNode(ExprNodePtr n1, ExprNodePtr n2, BinaryOpCode boc)
    : ExprNode(NodeType_BinOpExpr)
    , op(boc)
    , n1(n1)
    , n2(n2)
  {}

  void accept(ExprNodeVisitor* visitor) { visitor->visitExpr(this); }

  BinaryOpCode op;
  ExprNodePtr  n1;
  ExprNodePtr  n2;
};

class QILANG_API  UnaryOpExprNode : public ExprNode {
public:
  UnaryOpExprNode(ExprNodePtr node, UnaryOpCode op)
    : ExprNode(NodeType_UOpExpr)
    , op(op)
    , n1(node)
  {}

  void accept(ExprNodeVisitor* visitor) { visitor->visitExpr(this); }

  UnaryOpCode op;
  ExprNodePtr n1;

};

class QILANG_API VarExprNode : public ExprNode {
public:
  explicit VarExprNode(const std::string &name)
    : ExprNode(NodeType_VarExpr)
    , value(name)
  {}

  void accept(ExprNodeVisitor* visitor) { visitor->visitExpr(this); }

  std::string value;
};

class QILANG_API ConstDataExprNode : public ExprNode {
public:
  explicit ConstDataExprNode(const ConstDataNodePtr& data)
    : ExprNode(NodeType_ConstDataExpr)
    , data(data)
  {}

  void accept(ExprNodeVisitor* visitor) { visitor->visitExpr(this); }

  ConstDataNodePtr data;
};

// ####################
// # CONST DATA Node
// ####################
class QILANG_API ConstDataNode : public Node {
public:
  explicit ConstDataNode(NodeType type)
    : Node(NodeKind_ConstData, type)
  {}

  virtual void accept(ConstDataNodeVisitor* visitor) = 0;
};

class QILANG_API BoolConstDataNode: public ConstDataNode {
public:
  explicit BoolConstDataNode(bool val)
    : ConstDataNode(NodeType_BoolData)
    , value(val)
  {}

  void accept(ConstDataNodeVisitor* visitor) { visitor->visitData(this); }

  bool value;
};

class QILANG_API IntConstDataNode: public ConstDataNode {
public:
  explicit IntConstDataNode(qi::uint64_t val)
    : ConstDataNode(NodeType_IntData)
    , value(val)
  {}

  void accept(ConstDataNodeVisitor* visitor) { visitor->visitData(this); }

  qi::uint64_t value;
};

class QILANG_API FloatConstDataNode: public ConstDataNode {
public:
  explicit FloatConstDataNode(double val)
    : ConstDataNode(NodeType_FloatData)
    , value(val)
  {}

  void accept(ConstDataNodeVisitor* visitor) { visitor->visitData(this); }

  double value;
};

class QILANG_API StringConstDataNode: public ConstDataNode {
public:
  explicit StringConstDataNode(const std::string& value)
    : ConstDataNode(NodeType_StringData)
    , value(value)
  {}

  void accept(ConstDataNodeVisitor* visitor) { visitor->visitData(this); }

  const std::string value;
};

class QILANG_API ListConstDataNode: public ConstDataNode {
public:
  explicit ListConstDataNode(const ConstDataNodePtrVector& values)
    : ConstDataNode(NodeType_ListData)
    , values(values)
  {}

  void accept(ConstDataNodeVisitor* visitor) { visitor->visitData(this); }

  ConstDataNodePtrVector values;
};

class QILANG_API TupleConstDataNode: public ConstDataNode {
public:
  explicit TupleConstDataNode(const ConstDataNodePtrVector& values)
    : ConstDataNode(NodeType_TupleData)
    , values(values)
  {}

  void accept(ConstDataNodeVisitor* visitor) { visitor->visitData(this); }

  ConstDataNodePtrVector values;
};

class QILANG_API DictConstDataNode: public ConstDataNode {
public:
  explicit DictConstDataNode(const ConstDataNodePtrPairVector& values)
    : ConstDataNode(NodeType_MapData)
    , values(values)
  {}

  void accept(ConstDataNodeVisitor* visitor) { visitor->visitData(this); }

  ConstDataNodePtrPairVector values;
};


// ####################
// # TYPE EXPR Node
// ####################
class QILANG_API TypeExprNode : public Node {
public:
  explicit TypeExprNode(NodeType type)
    : Node(NodeKind_TypeExpr, type)
  {}

  virtual void accept(TypeExprNodeVisitor* visitor) = 0;
};

class QILANG_API SimpleTypeExprNode : public TypeExprNode {
public:
  explicit SimpleTypeExprNode(const std::string& sym)
    : TypeExprNode(NodeType_SimpleTypeExpr)
    , value(sym)
  {}

  void accept(TypeExprNodeVisitor* visitor) { visitor->visitTypeExpr(this); }

  std::string value;
};

class QILANG_API ListTypeExprNode : public TypeExprNode {
public:
  explicit ListTypeExprNode(const TypeExprNodePtr& element)
    : TypeExprNode(NodeType_ListTypeExpr)
    , element(element)
  {}

  void accept(TypeExprNodeVisitor* visitor) { visitor->visitTypeExpr(this); }

  TypeExprNodePtr element;
};

class QILANG_API MapTypeExprNode : public TypeExprNode {
public:
  explicit MapTypeExprNode(const TypeExprNodePtr& key, const TypeExprNodePtr& value)
    : TypeExprNode(NodeType_MapTypeExpr)
    , key(key)
    , value(value)
  {}

  void accept(TypeExprNodeVisitor* visitor) { visitor->visitTypeExpr(this); }

  TypeExprNodePtr key;
  TypeExprNodePtr value;
};

class QILANG_API TupleTypeExprNode : public TypeExprNode {
public:
  explicit TupleTypeExprNode(const TypeExprNodePtrVector& elements)
    : TypeExprNode(NodeType_TupleTypeExpr)
    , elements(elements)
  {}

  void accept(TypeExprNodeVisitor* visitor) { visitor->visitTypeExpr(this); }

  TypeExprNodePtrVector elements;
};


// ####################
// # STMT Node
// ####################
class QILANG_API StmtNode : public Node
{
public:
  StmtNode(NodeType type)
    : Node(NodeKind_Stmt, type)
  {}

  virtual void accept(StmtNodeVisitor* visitor) = 0;
};

class QILANG_API PackageNode : public StmtNode {
public:
  explicit PackageNode(const std::string& packageName)
    : StmtNode(NodeType_Package)
    , name(packageName)
  {}

  void accept(StmtNodeVisitor* visitor) { visitor->visitStmt(this); }

public:
  std::string name;
};

class QILANG_API ImportNode : public StmtNode {
public:
  explicit ImportNode(const std::string& packageName)
    : StmtNode(NodeType_Import)
    , name(packageName)
  {}

  ImportNode(const std::string& packageName, const StringVector& imported)
    : StmtNode(NodeType_Import)
    , name(packageName)
    , imported(imported)
  {
    if (imported.size() == 0)
      throw std::runtime_error("Empty import list");
  }

  void accept(StmtNodeVisitor* visitor) { visitor->visitStmt(this); }

public:
  std::string  name;
  StringVector imported;
};



class QILANG_API VarDefNode : public StmtNode {
public:
  VarDefNode(const std::string& name, const TypeExprNodePtr& type, const ConstDataNodePtr& data)
    : StmtNode(NodeType_VarDef)
    , name(name)
    , type(type)
    , data(data)
  {}

  VarDefNode(const std::string& name, const TypeExprNodePtr& type)
    : StmtNode(NodeType_VarDef)
    , name(name)
    , type(type)
  {}

  void accept(StmtNodeVisitor* visitor) { visitor->visitStmt(this); }

  std::string      name;
  TypeExprNodePtr  type;
  ConstDataNodePtr data;
};

// Object Motion.MoveTo "titi"
class QILANG_API ObjectDefNode : public StmtNode {
public:
  ObjectDefNode(const TypeExprNodePtr& type, const std::string& name, const StmtNodePtrVector& defs)
    : StmtNode(NodeType_ObjectDef)
    , type(type)
    , name(name)
    , values(defs)
  {}

  void accept(StmtNodeVisitor* visitor) { visitor->visitStmt(this); }

  TypeExprNodePtr   type;
  std::string       name;
  StmtNodePtrVector values;
};

// myprop: tititoto
class QILANG_API PropertyDefNode : public StmtNode {
public:
  PropertyDefNode(const std::string& name, ConstDataNodePtr data)
    : StmtNode(NodeType_PropDef)
    , name(name)
    , data(data)
  {}

  void accept(StmtNodeVisitor* visitor) { visitor->visitStmt(this); }

  std::string      name;
  ConstDataNodePtr data;
};

class QILANG_API AtNode : public StmtNode {
public:
  AtNode(const std::string& sender, const std::string& receiver)
    : StmtNode(NodeType_At)
    , sender(sender)
    , receiver(receiver)
  {}

  void accept(StmtNodeVisitor* visitor) { visitor->visitStmt(this); }

public:
  std::string sender;
  std::string receiver;
};

// ####################
// # DECL Node
// ####################

class QILANG_API DeclNode : public Node
{
public:
  DeclNode(NodeType type)
    : Node(NodeKind_Decl, type)
  {}

  virtual void accept(DeclNodeVisitor* visitor) = 0;
};

class QILANG_API FieldDeclNode : public DeclNode {
public:
  FieldDeclNode(const std::string &name, const TypeExprNodePtr& type)
    : DeclNode(NodeType_FieldDecl)
    , name(name)
    , type(type)
  {}

  void accept(DeclNodeVisitor* visitor) { visitor->visitDecl(this); }

  std::string     name;
  TypeExprNodePtr type;
};
typedef boost::shared_ptr<FieldDeclNode> FieldDeclNodePtr;
typedef std::vector<FieldDeclNodePtr>    FieldDeclNodePtrVector;

class QILANG_API StructDeclNode : public DeclNode {
public:
  StructDeclNode(const std::string& pkg, const std::string& name, const FieldDeclNodePtrVector& vardefs)
    : DeclNode(NodeType_StructDecl)
    , package(pkg)
    , name(name)
    , fields(vardefs)
  {}

  void accept(DeclNodeVisitor* visitor) { visitor->visitDecl(this); }

  std::string            package;
  std::string            name;
  FieldDeclNodePtrVector fields;
};

// Object Motion.MoveTo "titi"
class QILANG_API InterfaceDeclNode : public DeclNode {
public:
  InterfaceDeclNode(const std::string& pkg, const std::string& name, const DeclNodePtrVector& decls)
    : DeclNode(NodeType_InterfaceDecl)
    , package(pkg)
    , name(name)
    , values(decls)
  {}

  InterfaceDeclNode(const std::string& pkg, const std::string& name, const StringVector& inherits, const DeclNodePtrVector& decls)
    : DeclNode(NodeType_InterfaceDecl)
    , package(pkg)
    , name(name)
    , values(decls)
    , inherits(inherits)
  {}

  void accept(DeclNodeVisitor* visitor) { visitor->visitDecl(this); }

  std::string       package;
  std::string       name;
  DeclNodePtrVector values;
  StringVector      inherits;
};

class QILANG_API FnDeclNode : public DeclNode {
public:
  FnDeclNode(const std::string& name, const TypeExprNodePtrVector& args, const TypeExprNodePtr& ret)
    : DeclNode(NodeType_FnDecl)
    , name(name)
    , args(args)
    , ret(ret)
  {}

  FnDeclNode(const std::string& name, const TypeExprNodePtrVector& args)
    : DeclNode(NodeType_FnDecl)
    , name(name)
    , args(args)
  {}

  void accept(DeclNodeVisitor* visitor) { visitor->visitDecl(this); }

public:
  std::string       name;
  TypeExprNodePtrVector   args;
  TypeExprNodePtr         ret;
};


class QILANG_API EmitDeclNode : public DeclNode {
public:
  EmitDeclNode(const std::string& name, const TypeExprNodePtrVector& args)
    : DeclNode(NodeType_EmitDecl)
    , name(name)
    , args(args)
  {}

  void accept(DeclNodeVisitor* visitor) { visitor->visitDecl(this); }

public:
  std::string           name;
  TypeExprNodePtrVector args;
};

class QILANG_API PropDeclNode : public DeclNode {
public:
  PropDeclNode(const std::string& name, const TypeExprNodePtrVector& args)
    : DeclNode(NodeType_PropDecl)
    , name(name)
    , args(args)
  {}

  void accept(DeclNodeVisitor* visitor) { visitor->visitDecl(this); }

public:
  std::string           name;
  TypeExprNodePtrVector args;
};

class QILANG_API ConstDeclNode : public DeclNode {
public:
  ConstDeclNode(const std::string& pkg, const std::string& name, const TypeExprNodePtr& type, const ConstDataNodePtr& data)
    : DeclNode(NodeType_ConstDecl)
    , package(pkg)
    , name(name)
    , type(type)
    , data(data)
  {}

  ConstDeclNode(const std::string& pkg, const std::string& name, const ConstDataNodePtr& data)
    : DeclNode(NodeType_ConstDecl)
    , package(pkg)
    , name(name)
    , data(data)
  {}


  void accept(DeclNodeVisitor* visitor) { visitor->visitDecl(this); }

  std::string      package;
  std::string      name;
  TypeExprNodePtr  type;
  ConstDataNodePtr data;
};

}

#endif // NODE_HPP
