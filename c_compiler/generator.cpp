/*
 * File:	generator.cpp
 *
 * Description:	This file contains the public and member function
 *		definitions for the code generator for Simple C.
 *
 *		Extra functionality:
 *		- putting all the global declarations at the end
 */

# include <sstream>
# include <iostream>
# include "generator.h"
# include "machine.h"
# include "tokens.h"


using namespace std;
static unsigned maxargs;
int goffset;

 Strings strings;

Label currentReturn;

Label::Label(){
  value = counter;
  counter++;
}

unsigned Label::counter = 0;

ostream &operator<<(ostream &ostr, const Label &lbl){
  return ostr << ".L" << lbl.value;
}

/*
 * Function:	operator <<
 *
 * Description:	Convenience function for writing the operand of an
 *		expression.
 */

ostream &operator <<(ostream &ostr, Expression *expr)
{
    return ostr << expr->_operand;
}

string chooseReg(Expression* e){
  if(e->type().size() == SIZEOF_LONG){
    return "%rax";
  }
  else if(e->type().size() == SIZEOF_INT){
    return "%eax";
  }
  else{
    return "%al";
  }
}

void If::generate(){
  _expr->generate();
  cout << "\tcmpl\t$0, " << _expr << endl;
  Label exit;
  if(_elseStmt == nullptr){
    cout << "\tje\t" << exit << endl;
    _thenStmt->generate();
    cout << exit << ":" << endl;
  }else{
    Label skip;
    cout << "\tje\t" << skip << endl;
    _thenStmt->generate();
    cout << "\tjmp\t" << exit << endl;
    cout << skip << ":" << endl;
    _elseStmt->generate();
    cout << exit << ":" << endl;
  }
}

void While::generate(){
  Label loop;
  Label exit;
  cout << loop << ":" << endl;
  _expr->generate();
  cout << "\tcmpl\t$0, " << _expr << endl;
  cout << "\tje\t" << exit << endl;
  _stmt->generate();
  cout << "\tjmp\t" << loop << endl;
  cout << exit << ":" << endl;
  
}

void Return::generate(){
  _expr->generate();
  cout << "\tmov\t" << _expr << ", %rax" << endl;
  cout << "\tjmp\t" << currentReturn << endl;
}

void String::generate()
{
  //cout << "total number of strings " << strings.size() << endl;
  //cout << "in string generate" << endl;
  //cout << "searching for " << _value << endl;
  stringstream ss;
  ss << "(" << (strings.find(_value)->second) << ")";
  _operand = ss.str();
}

void Expression::generate(bool & indirect)
{
  indirect = false;
  generate();
}

void Dereference::generate()
{
  
  _expr->generate();
  cout << "\tmovq\t" << _expr << ", %rax" << endl;
  stringstream st;
  string temp;
  if(_type.size() == SIZEOF_LONG){
    cout << "\tmov\t(%rax), %rcx" << endl;
    goffset -= 8;
    st << goffset << "(%rbp)";
    _operand = st.str();
    cout << "\tmov\t%rcx, " << _operand << endl;
  }
  else if(_type.size() == SIZEOF_INT){
    cout << "\tmovl\t(%rax), %eax" << endl;
    goffset -= 4;
    st << goffset << "(%rbp)";
    _operand = st.str();
    cout << "\tmov\t%eax, " << _operand << endl;
  }
  else{
    cout << "\tmovb\t(%rax), %al" << endl;
    goffset -= 1;
    st << goffset << "(%rbp)";
    _operand = st.str();
    cout << "\tmov\t%al, " << _operand << endl;
  }
}

void Dereference::generate(bool &indirect)
{
  indirect = true;
  _expr->generate();
  _operand = _expr->_operand;
}

void Address::generate(){
  bool indirect;
  _expr->generate(indirect);
  if(indirect){
    _operand = _expr->_operand;
  }else{
    cout << "\tleaq\t" << _expr << ", %rax" << endl;
    goffset -= 8;
    stringstream ss;
    ss << goffset << "(%rbp)";
    _operand = ss.str();
    cout << "\tmovq\t%rax, " << _operand << endl;
  }
}

void Not::generate(){
  _expr->generate();
  string reg = chooseReg(_expr);
  cout << "\tmov\t" << _expr << ", " << reg << endl;
  cout << "\tcmp\t$0, " << reg << endl;
  cout << "\tsete\t%bl" << endl;
  if(_expr->type().size() == SIZEOF_LONG) cout << "\tmovzbq\t%bl, " << reg << endl;
  else cout << "\tmovzbl\t%bl, " << reg << endl;
  goffset -= _expr->type().size();
  stringstream ss;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmov\t" << reg << ", " << _operand << endl;
}

void Negate::generate()
{
  _expr->generate();
  string reg = chooseReg(_expr);
  cout << "\tmov\t" << _expr << ", " << reg << endl;
  cout << "\tneg\t" << reg << endl;
  stringstream ss;
  goffset -= _expr->type().size();
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout<< "\tmov\t" << reg << ", " << _operand << endl;
}
  

void Character::generate()
{
  stringstream ss;
  char c = _value[1];//ignore the single quotes
  int i = c;
  ss << "$" << i;
  _operand = ss.str();
}

void Cast::generate()
{
  _expr->generate();
  //cout << "in cast generate" << endl;
  stringstream ss;
  if(_expr->_operand[0] == '$'){
    //cout << "detected literal" << endl;
    _operand = _expr->_operand;
  }else if(_type.size() == _expr->type().size()){
    //cout << "detected same size" <<endl;
    _operand = _expr->_operand;
  }else if(_expr->type().size() == SIZEOF_CHAR){
    cout << "\tmov\t" << _expr << ", %al" << endl;
    if(_type.size() == SIZEOF_INT){
      cout << "\tmovsbl\t%al, %eax" << endl;
      goffset -= 4;
      ss << goffset << "(%rbp)";
      _operand = ss.str();
      cout << "\tmov\t%eax, " << _operand << endl;
    }else{
      cout << "\tmovsbq\t%al, %rax" << endl;
      goffset -= 8;
      ss << goffset << "(%rbp)";
      _operand = ss.str();
      cout << "\tmov\t%rax, " << _operand << endl;
    }
  }else if(_expr->type().size() == SIZEOF_INT){
    //cout << "detected int type" << endl;
    cout << "\tmov\t" << _expr << ", %eax" << endl;
    if(_type.size() == SIZEOF_CHAR){
      goffset -= 1;
      ss << goffset << "(%rbp)";
      _operand = ss.str();
      cout << "\tmovb\t%al, " << _operand << endl;
    }else{
      cout << "\tcltd" << endl;
      goffset -= 8;
      ss << goffset << "(%rbp)";
      _operand = ss.str();
      cout << "\tmovq\t%rax, " << _operand << endl;
    }
  }else if(_expr->type().size() == SIZEOF_LONG){
    cout << "\tmov\t" << _expr << ", %rax" << endl;
    if(_type.size() == SIZEOF_CHAR){
      goffset -= 1;
      ss << goffset << "(%rbp)";
      _operand = ss.str();
       cout << "\tmovb\t%al, " << _operand << endl;
    }else{
      goffset -= 4;
      ss << goffset << "(%rbp)";
      _operand = ss.str();
      cout << "\tmovl\t%eax, " << _operand << endl;
    }
  }
}
void Remainder::generate()
{
  _left->generate();
  _right->generate();
  string reg_left = chooseReg(_left);
  cout << "\tmov\t" << _left << ", " << reg_left << endl;
  stringstream ss;
  if(_type.size() == SIZEOF_LONG){
    cout << "\tcqto" << endl;
    cout << "\tmov\t" << _right << ", %rcx" << endl;
    cout << "\tidiv\t%rcx" << endl;
    goffset -= 8;
    ss << goffset << "(%rbp)";
    _operand = ss.str();
    cout << "\tmov\t%rdx, " << _operand << endl;
  }
  else{
    cout << "\tcltd" << endl;
    cout << "\tmov\t" << _right << ", %ecx" << endl;
    cout << "\tidiv\t%ecx" << endl;
    goffset -= 4;
    ss << goffset << "(%rbp)";
    _operand = ss.str();
    cout << "\tmov\t%edx, " << _operand << endl;
  }
}

void Divide::generate()
{
  _left->generate();
  _right->generate();
  string reg_left = chooseReg(_left);
  cout << "\tmov\t" << _left << ", " << reg_left << endl;
  stringstream ss;
  if(_type.size() == SIZEOF_LONG){
    cout << "\tcqto" << endl;
    cout << "\tmov\t" << _right << ", %rcx" << endl;
    cout << "\tidiv\t%rcx" << endl;
    goffset -= 8;
  }
  else{
    cout << "\tcltd" << endl;
    cout << "\tmov\t" << _right << ", %ecx" << endl;
    cout << "\tidiv\t%ecx" << endl;
    goffset -= 4;
  }
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmov\t" << reg_left << ", " << _operand << endl;
}

void Multiply::generate()
{
  _left->generate();
  _right->generate();
  string reg_left = chooseReg(_left);
  cout << "\tmov\t" << _left << ", " << reg_left << endl;
  cout << "\timul\t" << _right << ", " << reg_left << endl;
  stringstream ss;
  if(_type.size() == SIZEOF_LONG){
    goffset -= 8;
  }
  else goffset -= 4;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmov\t" << reg_left << ", " << _operand << endl;
}

void Subtract::generate()
{
  _left->generate();
  _right->generate();
  string reg_left = chooseReg(_left);
  cout << "\tmov\t" << _left << ", " << reg_left << endl;
  cout << "\tsub\t" << _right << ", " << reg_left << endl;
  stringstream ss;
  if(_type.size() == SIZEOF_LONG){
    goffset -= 8;
  }
  else goffset -= 4;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmov\t" << reg_left << ", " << _operand << endl;
}
	Label l;

void Add::generate()
{
  _left->generate();
  _right->generate();
  string reg_left = chooseReg(_left);
  cout << "\tmov\t" << _left << ", " << reg_left << endl;
  cout << "\tadd\t" << _right << ", " << reg_left << endl;
  stringstream ss;
  if(_type.size() == SIZEOF_LONG){
    goffset -= 8;
  }
  else goffset -= 4;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmov\t" << reg_left << ", " << _operand << endl;
}

void GreaterThan::generate()
{
  _left->generate();
  _right->generate();
  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetg\t%al" << endl;
  cout << "\tmovzbl\t%al, %eax" << endl;
  stringstream ss;
  goffset -= 4;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void LessThan::generate()
{
  _left->generate();
  _right->generate();
  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetl\t%al" << endl;
  cout << "\tmovzbl\t%al, %eax" << endl;
  stringstream ss;
  goffset -= 4;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void GreaterOrEqual::generate()
{
  _left->generate();
  _right->generate();
  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetge\t%al" << endl;
  cout << "\tmovzbl\t%al, %eax" << endl;
  stringstream ss;
  goffset -= 4;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void LessOrEqual::generate()
{
  _left->generate();
  _right->generate();
  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetle\t%al" << endl;
  cout << "\tmovzbl\t%al, %eax" << endl;
  stringstream ss;
  goffset -= 4;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void NotEqual::generate()
{
  _left->generate();
  _right->generate();
  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsetne\t%al" << endl;
  cout << "\tmovzbl\t%al, %eax" << endl;
  stringstream ss;
  goffset -= 4;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void Equal::generate()
{
  _left->generate();
  _right->generate();
  cout << "\tmovl\t" << _left << ", %eax" << endl;
  cout << "\tcmpl\t" << _right << ", %eax" << endl;
  cout << "\tsete\t%al" << endl;
  cout << "\tmovzbl\t%al, %eax" << endl;
  stringstream ss;
  goffset -= 4;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void LogicalAnd::generate()
{
  _left->generate();
  cout << "\tcmpl\t$0, " << _left << endl;
  Label labelor;
  cout << "\tje\t" << labelor << endl;
  _right->generate();
  cout << "\tcmpl\t$0, " << _right << endl;
  cout << labelor << ":" << endl;
  cout << "\tsetne\t%al" << endl;
  goffset -= 4;
  stringstream ss;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmovzbl\t%al, %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
}

void LogicalOr::generate()
{
  _left->generate();
  cout << "\tcmpl\t$0, " << _left << endl;
  Label labelor;
  cout << "\tjne\t" << labelor << endl;
  _right->generate();
  cout << "\tcmpl\t$0, " << _right << endl;
  cout << labelor << ":" << endl;
  cout << "\tsetne\t%al" << endl;
  goffset -= 4;  
  stringstream ss;
  ss << goffset << "(%rbp)";
  _operand = ss.str();
  cout << "\tmovzbl\t%al, %eax" << endl;
  cout << "\tmovl\t%eax, " << _operand << endl;
  
}

static string paramRegs[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
static string paramRegsLong[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};



/*
 * Function:	Identifier::generate
 *
 * Description:	Generate code for an identifier.  Since there is really no
 *		code to generate, we simply update our operand.
 */

void Identifier::generate()
{
    stringstream ss;


    if (_symbol->_offset != 0)
	ss << _symbol->_offset << "(%rbp)";
    else
	ss << target->global_prefix << _symbol->name() << target->global_suffix;

    _operand = ss.str();
}


/*
 * Function:	Number::generate
 *
 * Description:	Generate code for a number.  Since there is really no code
 *		to generate, we simply update our operand.
 */

void Number::generate()
{
    stringstream ss;

    
    if(_value.at(_value.size() - 1) == 'l') _value = _value.substr(0, _value.size() - 1);

    ss << "$" << _value;
    _operand = ss.str();
}


/*
 * Function:	Call::generate
 *
 * Description:	Generate code for a function call expression, in which each
 *		argument is simply a variable or an integer literal.
 */

void Call::generate()
{
    if (_args.size() > maxargs)
	maxargs = _args.size();

    for (unsigned i = 0; i < _args.size(); i ++)
	_args[i]->generate();

    for (unsigned i = 0; i < _args.size(); i ++) {

	if (i < NUM_ARGS_IN_REGS) {
	    if(_args[i]->type().size() == SIZEOF_LONG){
	      
	      cout << "\tmovq\t" << _args[i]->_operand << ", ";
	      cout << paramRegsLong[i] << endl;
	    }
	    else{
	      cout << "\tmovl\t" << _args[i]->_operand << ", ";
	      cout << paramRegs[i] << endl;
	    }
	} else {
	    if(_args[i]->type().size() == SIZEOF_LONG){
	      cout << "\tmovq\t" << _args[i]->_operand << ", %rax" << endl;
	      cout << "\tmovq\t%rax, ";
	      cout << (i - NUM_ARGS_IN_REGS) * SIZEOF_ARG << "(%rsp)" << endl;
	    }
	    else{
	      cout << "\tmovl\t" << _args[i]->_operand << ", %eax" << endl;
	      cout << "\tmovq\t%rax, ";
	      cout << (i - NUM_ARGS_IN_REGS) * SIZEOF_ARG << "(%rsp)" << endl;
	    }
	}
    }
    
    if(_id->type().parameters() == nullptr) cout << "\tmov\t$0, %eax" << endl;

    cout << "\tcall\t" << target->global_prefix << _id->name() << endl;
    
    stringstream ss;
    if((_id->type().indirection() > 0) || (_id->type().specifier() == LONG)){
      goffset -= 8;
      ss << goffset << "(%rbp)";
      _operand = ss.str();
      cout << "\tmov\t%rax, " << _operand << endl;
    }
    else if(_id->type().specifier() == INT){
      goffset -= 4;
      ss << goffset << "(%rbp)";
      _operand = ss.str();
      cout << "\tmov\t%eax, " << _operand << endl;
    }
    else{
      goffset -= 1;
      ss << goffset << "(%rbp)";
      _operand = ss.str();
      cout << "\tmov\t%al, " << _operand << endl;
    }
}


/*
 * Function:	Assignment::generate
 *
 * Description:	Generate code for this assignment statement, in which the
 *		right-hand side is an integer literal and the left-hand
 *		side is an integer scalar variable.  Actually, the way
 *		we've written things, the right-side can be a variable too.
 */

void Assignment::generate()
{
    //cout << "calling assignment left generate" << endl;
    bool indirect = false;;
    
    _right->generate();
    _left->generate(indirect);
    //cout << "calling assignment right generate" << endl;
    //cout << "now generating assignment" << endl;

    
    string reg = chooseReg(_left);
    cout << "\tmov\t" << _right << ", " << reg << endl;
    if(indirect){
      cout << "\tmovq\t" << _left << ", %rcx" << endl;
      cout << "\tmov\t" << reg << ", (%rcx)" << endl;
    }else{
      cout << "\tmov\t" << reg << ", " << _left << endl;
    }
}


/*
 * Function:	Block::generate
 *
 * Description:	Generate code for
    virtual void generate(); this block, which simply means we
 *		generate code for each statement within the block.
 */

void Block::generate()
{
    for (unsigned i = 0; i < _stmts.size(); i ++)
	_stmts[i]->generate();
}


/*
 * Function:	Function::generate
 *
 * Description:	Generate code for this function, which entails allocating
 *		space for local variables, then emitting our prologue, the
 *		body of the function, and the epilogue.
 */

void Function::generate()
{
    Label l;
    currentReturn = l;
    long offset;
    Symbols symbols = _body->declarations()->symbols();
    unsigned numParams = _id->type().parameters()->size();


    /* Generate our prologue. */

    offset = 0;
    maxargs = 0;

    allocate(offset);
    cout << target->global_prefix << _id->name() << ":" << endl;
    cout << "\tpushq\t%rbp" << endl;
    cout << "\tmovq\t%rsp, %rbp" << endl;

    /*
    if (target == &OS_X) {
	cout << "\tmovl\t$" << _id->name() << ".size, %eax" << endl;
	cout << "\tsubq\t%rax, %rsp" << endl;
    } else
    */
	cout << "\tsubq\t$" << _id->name() << ".size, %rsp" << endl;

    if (numParams > NUM_ARGS_IN_REGS)
	numParams = NUM_ARGS_IN_REGS;
    for (unsigned i = 0; i < numParams; i ++) {
      if(_id->type().parameters()->at(i).size() == SIZEOF_LONG){
	cout << "\tmovq\t" << paramRegsLong[i] << ", ";
	cout << symbols[i]->_offset << "(%rbp)" << endl;
      }else{
	cout << "\tmovl\t" << paramRegs[i] << ", ";
	cout << symbols[i]->_offset << "(%rbp)" << endl;
      }
    }


    /* Generate the body of this function. */

     goffset = offset;
    _body->generate();
     offset = goffset;


    /* Finish aligning the stack */

    if (maxargs > NUM_ARGS_IN_REGS)
	offset -= (maxargs - NUM_ARGS_IN_REGS) * SIZEOF_ARG;

    while (offset % STACK_ALIGNMENT != 0)
	offset --;

    cout << currentReturn << ":" << endl;
    /* Generate our epilogue. */

    cout << "\tmovq\t%rbp, %rsp" << endl;
    cout << "\tpopq\t%rbp" << endl;
    cout << "\tret" << endl << endl;

    cout << "\t.globl\t" << target->global_prefix << _id->name() << endl;
    cout << "\t.set\t" << _id->name() << ".size, " << -offset << endl;

    cout << endl;
}


/*
 * Function:	generateGlobals
 *
 * Description:	Generate code for any global variable declarations.
 */

void generateGlobals(const Symbols &globals)
{
    for (Strings::const_iterator it = strings.begin(); it != strings.end(); ++it)
    {
      cout << it->second << ": .asciz " << it->first  << endl;
    }
  
    if (globals.size() > 0)
	cout << "\t.data" << endl;

    for (unsigned i = 0; i < globals.size(); i ++) {
	cout << "\t.comm\t" << target->global_prefix << globals[i]->name();
	cout << ", " << globals[i]->type().size() << endl;
    }
}
