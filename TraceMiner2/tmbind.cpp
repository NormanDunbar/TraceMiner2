#include "tmbind.h"

/** @brief Constructor for a tmBind object.
 *
 * @param id unsigned. The positional bind number. The first  bind in a SQL statement is bind 0, and so on.
 * @param name std::string. The extracted bind variable name. May be wrapped in double quotes, maybe not.
 *
 */
tmBind::tmBind(unsigned id, string name) {

    mBindId = id;
    mBindLineNumber = 0;
    mBindValue = "";
    mBindName = name;
}

/** @brief Destructor for a tmBind object.
 */
tmBind::~tmBind()
{
    //dtor
}


/** @brief Allows a tmBind to be streamed to an ostream.
 *
 * @param out ostream&. The stream to output tmBind details to.
 * @param bind tmBind&. The tmBind to be streamed.
 * @return ostream&. The same as the input stream.
 *
 * This function/operator allows a tmBind to stream itself
 * out to an ostream.
 */
ostream &operator<<(ostream &out, tmBind &bind) {
    out << "BindID: " << bind.BindId() << endl;
    out << "Bind Line Number: " << bind.BindLineNumber() << endl;
    out << "Bind Name: " << bind.BindName() << endl;
    out << "Bind Value: " << bind.BindValue() << endl << endl;
    return out;
}

