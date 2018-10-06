#if !defined(DT_H)
#define DT_H
/**
 * @brief The Dependency Tree. Parses the AST and looks at the pipe-based
 * dependencies of each statement
 * 
 */

#include <map>
#include <string>

#include "ast.h"

using std::map;
using std::string;

class DT { 
	public:		
	DT(NBlock* root);

	map<string, string> idToFunc;
	map<string, vector<NFunction*>> depToFuncs;
}


#endif // DT_H
