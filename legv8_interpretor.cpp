#include <iostream>
#include <algorithm>
#include <string>
#include <deque>
#include <map>
#include <cctype>

using namespace std;


string toLower(string s)
{
	string new_string;
	for (int i = 0; i < s.size(); ++i)
	{
	      new_string += tolower(s[i]);
   	}
   	return new_string;
}

/*
int getRegisterIdOrIntermediateValue(string register_name)

	
	this will return an integer between 0 and 32

	if register_name == "sp"
		return 31
	if the register name says like "x12" then you will return 12
	if the register name says like "x22" then you will return 22


	if the register name says like "xzr" then you will return 32
	if the register name says like "lr" then you will return 30   (link register)


	"x12" is encoded in ascii so you will have to convert the ascii value of "1", and "2" to 1 and 2

	have it also get intermediate values like "#234562" -> 234562

*/

/*

assumes you already converted from ascii
the array contains only integers
take an array [2, 3, 4, 5, 6, 2] and return 234562
iterations of algorithm

start with 2
20 + 3 = 23
230 + 4 = 234


you can do this directly from the string by taking each ascii char and subtracting 48 from it(I think the value is 48)
no need to use an integer array




registers[getRegisterIdOrIntermediateValue("sp")]

*/


/*

subi sp, sp, #number will need to record the distance from the sp to the new sp
so b lr can go back to sp's original location in stack
*/


































string getRidOfCharacter(string input, char character_to_remove)
{
	string new_string;
	int i = 0;
	while(i < input.size())
	{
		if(input[i] != character_to_remove)
		{
			new_string += input[i];
		}
		i++;
	}
	return new_string;
}


deque<string>* tokenize(string input, string commands[])
{
	int j = 0;
	deque<string>* tokens = new deque<string>;
	string new_input = toLower(input);
	/*
	cout << input << endl;
	for(int i = 0; i < 15; i++)
	{
		cout << commands[i] << endl;
	}
	*/
	cout << new_input << endl;
	int count = 0;
	while(j < new_input.size())
	{
		if(count >= 10)
			exit(1);
		//printf("j = %i\n", j);
		// tries all possible tokens
		// if a match
		// collect and bump up j
		bool found = false;

		// comma is i == 45? so why does this work?
		for(int i = 0; i < 45; i++)
		{
			//printf("%s  %i \n", &commands[i], j);
			//cout << tests[0].substr(j, commands[i].size()) << "|"<< endl;
			//cout << commands[i] << "|" << endl << endl;
			//cout << (input.substr(j, commands[i].size()) == commands[i]) << found << endl;
			if(new_input.substr(j, commands[i].size()) == commands[i])
			{
				//printf("got here\n");
				//cout << "found" << "|"<<commands[i]<< "|" << endl;
				tokens->push_back(commands[i]);
				//printf("got here 2\n");

				j += commands[i].size();
				found = true;
				//for_each(tokens.begin(), tokens.end(), [](string item){cout << item << "|";});
				//cout << endl;
				break;
			}
			

		}
		//cout << found << endl;

		cout << "j " << j << endl;
		// otherwise check for a prefix of the longer tokens and longer set of short tokens
		if(new_input[j] == '#' || new_input[j] == 'x' || new_input[j] == 'X')
		{
			int offset = j;
			offset++;
			int size = 0;

			while((new_input[offset] >= '0' && new_input[offset] <= '9') ||
				  (new_input[offset] >= 'a' && new_input[offset] <= 'f') ||
				  (new_input[offset] >= 'A' && new_input[offset] <= 'F'))
			{
				offset++;
				size++;

			}
			// should be #654323456543
			tokens->push_back(new_input.substr(j, size + 1));
			cout << "token found\n";
			cout << tokens->at(tokens->size() - 1) << endl;
			j = offset;

		}

		//cout << "j2 " << j << endl;


		// prefixes: "#", "0x", "x", "X"
		// save prefix and the digits comming after it
			// save it and increment
		//cout << found << endl;
		//printf("j = %i\n", j);
		// the input started with a label
		
		if(!found)
		{
			cout << "here\n";
			// it is a label so grab the rest
			//cout << tests[0].size() << " " << j << endl;
			tokens->push_back(new_input.substr(j, new_input.size() - j));
			j = new_input.size();
		}
		for_each(tokens->begin(), tokens->end(), [](string item){cout << item << "|";});

		count++;

	}
	return tokens;
}
string getRidOfLabel(string line_starting_with_a_label)
{
	// find location of label
	// cut it off
	// 
	string label;

	int end = line_starting_with_a_label.find(":");
	if(end != std::string::npos)
	{
		// +1 for the ":"
		label = line_starting_with_a_label.substr(end + 1, line_starting_with_a_label.size() - end);

	}
	return label;

}
string getLabel(string line_starting_with_a_label)
{
	string label;
	int end = line_starting_with_a_label.find(":");
	if(end != std::string::npos)
	{
		label = line_starting_with_a_label.substr(0, end);
	}
	return label;
}
string getRidOfLeftParens(string token)
{
	if(token[0] == '[')
	{
		//cout << token<< " [\n";

		token = token.substr(1, token.size() - 2);
		//cout << token << endl;

	}
	return token;
}

string getRidOfCommaOnRightSide(string token)
{

	if(token[token.size() - 1] == ',')
	{
		//cout << token << " ,\n";

		token = token.substr(0, token.size() - 1);
		//cout << token << endl;
	}
	return token;
}
string getRidOfRightParenseOnRightSide(string token)
{

	if(token[token.size() - 1] == ']')
	{
		//cout << token << " ]\n";

		token = token.substr(0, token.size() - 1);
		//cout << token << endl;
	}
	return token;
}

deque<string>* split(string input)
{
	// assume all keywords+ a little punctuation are separated out by whitespace
	deque<string>* keyword_puncs = new deque<string>;
	string keyword_punc;
	int i = 0;
	while(i < input.size())
	{
		if(input[i] != ' ')
		{
			keyword_punc += tolower(input[i]);
		}
		else
		{
			keyword_puncs->push_back(keyword_punc);
			keyword_punc = "";
		}
		i++;
	}
	if(keyword_punc.size() > 0)
	{
		keyword_puncs->push_back(keyword_punc);

	}
	return keyword_puncs;
}
int main()
{

	
	string tests2[] = {	"Start:", "b.eq ,label",
						"another_label:",
						"another_label2:",
						"label:  ldurx0[sp#8]",
						"label_7:add x0, x0, x12",
						"addi x0, x4, #32345678",
						"b.eq another_label",
						"b Start"
					};
	
	string tests[] = {
		"ADDI X0, X31, #12",
		"FUN: CBZ X0, DONE",
		"CMPI X0, #1",
		"B.EQ DONE",
		"SUBI SP, SP, #32",
		"STUR X0, [SP, #0]",
		"STUR X30, [SP, #8]",
		"STUR X19, [SP, #16]",
		"SUBI X0, X0, #1",
		"BL FUN",
		"ADDI X19, X0, #0",
		"LDUR X0, [SP, #0]",
		"SUBI X0, X0, #2",
		"BL FUN",
		"ADD X0, X0, X19",
		"LDUR X30, [SP, #8]",
		"LDUR X19, [SP, #16]",
		"ADDI SP, SP, #32",
		"DONE: BR X30"

	};
	for(int i = 0; i < 19; i++)
	{
		deque<string>* tokens2 = split(tests[i]);
		//for_each(tokens2->begin(), tokens2->end(), [](string item){cout << item << "|";});
		//cout << endl << endl;

	}
	//return 0;

	/*
ADDI X0, X31, #12 FUN: CBZ X0, DONE
CMPI X0, #1
B.EQ DONE
SUBI SP, SP, #32 STUR X0, [SP, #0] STUR X30, [SP, #8] STUR X19, [SP, #16] SUBI X0, X0, #1
BL FUN
ADDI X19, X0, #0 LDUR X0, [SP, #0] SUBI X0, X0, #2 BL FUN
ADD X0, X0, X19 LDUR X30, [SP, #8] LDUR X19, [SP, #16] ADDI SP, SP, #32 // (*)
DONE: BR X30
	*/
	// make sure longer variations of the word start first

	string tokens[] = {

		"b.eq",
		"b.lt",

		"b.gt",
		"b.lo",
		
		"b.hi",
		"b.ne",
		
		"b.le",
		"b.ge",
		
		"b.ls",
		"b.hs",
		
		"bl",
		"br",
		
		"b",
		"sturb",
		
		"sturw",
		"subis",
		
		"sturh",
		"stxr",
		
		"stur",
		"subs",
		
		"subi",
		"sub",
		
		"ldursw",
		"ldurb",
		
		"ldurh",
		"ldxr",
		
		"ldur",
		"lsr",
		
		"lsl",
		"orri",
		
		"orr",
		"eori",
		
		"eor",
		"andis",
		
		"addi",
		"andi",
		
		"ands",
		"adds",
		
		"and",
		"add",
		
		"cbnz",
		"cbz",
		
		"[",
		"sp",
		
		"]",
		","
	};
	/*
	b
	sturb
	ldurb
	b.cond
	orri
	eori
	sturh
	ldurh
	has to do with hight and low bits for signed or unsigned
	and
	add
	addi
	andi
	bl
	orr
	adds
	cbz
	cbnz
	sturw
	ldursw
	stxr
	ldxr
	eor
	sub
	subi
	lsr
	lsl
	br
	ands
	subs
	subis
	andis
	stur
	ldur


	b.cond,
	bl,
	br,
	b,

	sturb,
	sturw,
	subis,
	sturh,
	stxr,
	stur,
	subs,
	subi,
	sub,

	ldursw,
	ldurb,
	ldurh,
	ldxr,
	ldur,
	lsr,
	lsl,


	orri,
	orr,

	eori,
	eor,

	
	andis,
	addi,
	andi,
	ands,
	adds,
	and,
	add,



	cbnz,
	cbz

	
	
	
	
	
	
	
	
	

	
	
	*/
	deque<deque<string>* > tests_token_lines;
	map<string, int> gotos;
	long long int mem[255];
	long long int registers[32];
	// need something for the STK memory
	// is it just this?
	long long int stack[255];
	int program_counter = 0;
	// will need functions to make the following sets of tokens "Xi", "#number", "number"
	// have the token function recognize hex values using "0x"
	// "#"
	// digit
	// x, X

	for(int i = 0; i < 19; i++)
	{
		deque<string>* tokens = split(tests[i]);
		deque<string>* trimmed_tokens = new deque<string>(tokens->size());

		transform(tokens->begin(), tokens->end(), trimmed_tokens->begin(), [](string item){
			return getRidOfRightParenseOnRightSide(getRidOfCommaOnRightSide(getRidOfLeftParens(item)));

		});
		for_each(trimmed_tokens->begin(), trimmed_tokens->end(), [](string item){cout << item << "|";});

		//cout << (tests[i].find(":") != std::string::npos) << endl;
		// if there is only one item left and it ends in : then it's a label
		if(tests[i].find(":") != std::string::npos)
		{
			// fun:|cbz|x0,|
			// done:|br|
			// done:
			string label;
			// get first element of tokens
			label = trimmed_tokens->at(0).substr(0, trimmed_tokens->at(0).size() - 1);
				cout << label<<"\n";

			trimmed_tokens->pop_front();
			if(trimmed_tokens->size() > 0)
			{
				gotos.insert(pair<string, int>(label, tests_token_lines.size()));
				tests_token_lines.push_back(trimmed_tokens);
				cout << label << " " << gotos[label] << endl;
			}
			else
			{
				gotos.insert(pair<string, int>(label, tests_token_lines.size()));
				cout << label << " " << gotos[label] << endl;

			}
			
		}
		else
		{
			tests_token_lines.push_back(trimmed_tokens);

		}
		//deque<string>* split_test_i = tokenize(tests[i], tokens);
		// there may be an instruction on the same line as the label
		// save it into the goto list
		// assuming there is no label that comes before its definition(this can be changed)
		//for_each(split_test_i->begin(), split_test_i->end(), [](string item){cout << item << " ";});
		cout << endl;
		

	}

	

	cout << endl;
	for_each(gotos.begin(), gotos.end(), [](pair<string, int> goto_){cout << goto_.first << " " << goto_.second;});
	return 0;

	// i should be the program counter
	// each deque inside tests_token_lines is a deque of the tokenzed strings from the original string
	for(int i = 0; i < tests_token_lines.size(); i++)
	{
		cout << i << endl;
		// skip over empty lines
		/*if(tests_token_lines.at(i)->size() == 0)
		{
			continue;
		}*/
		for_each(tests_token_lines.at(i)->begin(), tests_token_lines.at(i)->end(), [](string item){cout << item << " ";});
		string command = tests_token_lines.at(i)->at(0);
		if(command == "b.eq")
		{
			cout << "->b.eq instruction" << endl;
			
			/*cout << "goto list\n";
			for(map<string, int>::iterator j = gotos.begin(); j != gotos.end(); j++)
			{
				cout << j->first << " => " << gotos.at(tests_token_lines.at(i)->at(0)) << endl;
			}*/
			
			cout << tests_token_lines.at(i)->at(1) << " -> i " << gotos.at(tests_token_lines.at(i)->at(1)) << endl;
			int location = gotos.at(tests_token_lines.at(i)->at(1));
			cout << "->jump to " << tests_token_lines.at(location)->at(0) << " "<< tests_token_lines.at(location)->at(1)  << " when flags say equal" << endl;
			// run a function called b.eq to affect the memory and registers however the instruction says
			// bEQ(split_test_i, mem, registers)
		}
		if(command == "b")
		{
			cout << "->b instruction" << endl;
			
			/*cout << "goto list\n";
			for(map<string, int>::iterator j = gotos.begin(); j != gotos.end(); j++)
			{
				cout << j->first << " => " << gotos.at(tests_token_lines.at(i)->at(0)) << endl;
			}*/
			
			cout << tests_token_lines.at(i)->at(1) << " -> i " << gotos.at(tests_token_lines.at(i)->at(1)) << endl;

			int location = gotos.at(tests_token_lines.at(i)->at(1));
			cout << "->jump to " << tests_token_lines.at(location)->at(0) << " "<< tests_token_lines.at(location)->at(1) << " when flags say equal" << endl;
			// run a function called b.eq to affect the memory and registers however the instruction says
			// b(split_test_i, mem, registers)
		}
		else if(command == "add")
		{
			cout << "->add instruction" << endl;
			cout << "->add p1 and p2 and store the result in p0" << endl;
			// add(split_test_i, mem, registers)

		}
		else if(command == "addi")
		{
			// ge rid of the commas on the end of the register names
			cout << "->addi instruction" << endl;
			cout << "->add p1 and number and store the result in p0" << endl;

		}

		// http://www.eecs.umich.edu/courses/eecs370/eecs370.w19/resources/materials/ARM-v8-Quick-Reference-Guide.pdf
		// https://www.ibm.com/support/knowledgecenter/en/ssw_aix_72/com.ibm.aix.alangref/idalangref_comp_immed.htm
		else if(command == "b.eq")
		{

			// beq(jump_label)
		}
		else if(command == "b.lt")
		{
			// blt(jump_label)
		}
		else if(command == "b.gt")
		{
			// bgt(jump_label)
		}
		else if(command == "b.lo")
		{
			// blo(jump_label)
		}
		else if(command == "b.hi")
		{
			// bhi(jump_label)
		}
		else if(command == "b.ne")
		{
			// bne(jump_label)
		}
		else if(command == "b.le")
		{
			// ble(jump_label)
		}
		else if(command == "b.ge")
		{
			// bge(jump_label)
		}
		else if(command == "b.ls")
		{
			// bls(jump_label)
		}
		else if(command == "b.hs")
		{
			// bhs(jump_label)
		}
		else if(command == "bl")
		{
			// set pc to jump_label line number and store line number into link register
			// bl(jump_label)
		}
		else if(command == "br")
		{
			// set pc to register
			// br(register)
		}
		else if(command == "b")
		{
			// set pc to jump_label line number or register value
			// b(jump_label_or_register)
		}
		else if(command == "sturb")
		{
			// sturb(register1, register2, offset_from_stack_pointer)
		}
		else if(command == "sturw")
		{
			// sturw(register1, register2, offset_from_stack_pointer)

		}
		else if(command == "sturh")
		{
			// sturw(register1, register2, offset_from_stack_pointer)
		}
		else if(command == "stxr")
		{
			// sturw(register1, register2, offset_from_stack_pointer)

		}
		else if(command == "stur")
		{
			// sturw(register1, register2, offset_from_stack_pointer)

		}
		else if(command == "subis")
		{
			// subis(register1, register2, immediate_value)

		}
		else if(command == "subs")
		{
			// subs(register1, register2, register3)
		}
		else if(command == "subi")
		{
			// subi(register1, register2, immediate_value)
		}
		else if(command == "sub")
		{
			// sub(register1, register2, register3)

		}
		else if(command == "ldursw")
		{
			// ldursw(register1, register2, offset_from_stack_pointer)

		}
		else if(command == "ldurb")
		{
			// ldurb(register1, register2, offset_from_stack_pointer)
		}
		else if(command == "ldurh")
		{
			// ldurh(register1, register2, offset_from_stack_pointer)
		}
		else if(command == "ldxr")
		{
			// ldxr(register1, register2, offset_from_stack_pointer)
		}
		else if(command == "ldur")
		{
			// ldur(register1, register2, shift_amount)
		}
		else if(command == "lsr")
		{
			// lsr(register1, register2, shift_amount)
		}
		else if(command == "lsl")
		{
			// lsl(register1, register2, shift_amount)
		}
		else if(command == "orri")
		{
			// orri(register1, register2, immediate_value)
		}
		else if(command == "orr")
		{
			// orr(register1, register2, register3)
		}
		else if(command == "eori")
		{
			// eori(register1, register2, immediate_value)

		}
		else if(command == "eor")
		{
			// eori(register1, register2, register3)

		}
		else if(command == "andis")
		{
			// andis(register1, register2, immediate_value)
		}
		else if(command == "addi")
		{
			// addi(register1, register2, immediate_value)
		}
		else if(command == "andi")
		{
			// andi(register1, register2, immediate_value)
		}
		else if(command == "ands")
		{
			// ands(register1, register2, register3)

		}
		else if(command == "adds")
		{
			// adds(register1, register2, register3)

		}
		else if(command == "and")
		{
			// and(register1, register2, register3)
		}
		else if(command == "add")
		{
			// add(register1, register2, register3)
		}
		else if(command == "cbnz")
		{
			// cbnz(register1, label)
		}
		else if(command == "cbz")
		{
			// cbz(register1, label)
		}
		else if(command == "cmp")
		{
			// cmp(register1, register2, register3)
		}
		else if(command == "cmpi")
		{
			// cmpi(register1, register2, immediate_value)
		}
		
		cout << endl;
	}
	// delete the inner deques
	/*
	for(map<string, int>::iterator i = gotos.begin(); i != gotos.end(); i++)
	{
		cout << i->first << " => " << i->second << " " << tests[i->second] << endl;
	}*/
	cout << endl;

	/*
	idea for nested functions
	stur to stack pointer pushes things to stack
	b label adjusts the stack pointer to make a new stack
	bl link_register to reset the stack register



	LEGv8 has a branch-and-link (BL) instruction:
 It saves the address of the following instruction (the return address) in register LR, and then branches to specified address
BL ProcedureAddress
 The BL command saves PC + 4 into register LR
push

To return from the procedure, the branch
register instruction is used: BR LR
 The BR instruction copies address stored in register LR to PC register
pop


if it says b or b.eq just goto it and don't assume its a function call


http://www.cas.mcmaster.ca/~leduc/slides2ga3/2GA3slides2.pdf
	*/
	return 0;
}