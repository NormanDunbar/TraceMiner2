#include "tmcursor.h"

/** @file tmcursor.cpp
 * @brief Implementation file for the tmCursor object.
 */


/** @brief Constructor for tmCursor object.
 *
 * @param	id std::string. The cursorID including leading '#'.
 * @param	sqlSize unsigned. The length of the SQL text.
 * @param	sqlLine unsigned. The line in the trace file where the SQL statement begins.
 * @return	None.
 */
 tmCursor::tmCursor(string id, unsigned sqlSize, unsigned sqlLine) {
    mCursorId = id;
    mSQLLineNumber = sqlLine;
    mSQLSize = sqlSize;
    mSQLText = "";
    mSQLParseLine = 0;
    mBindCount = 0;

}

/** @brief Destructor for tmCursor object.
 */
tmCursor::~tmCursor() {
    // Anything to do to kill a tmCursor?
    // Of course there is, get shot of all the binds in the map.
    cleanUp();
}


/** @brief Cleans up on destruction of a tmCursor and on changing the SQL.
 *
 * When a tmCursor is destroyed, we must clean up all the assigned tmBinds
 * from the map. This function is also called when the SQL Text is changed
 * on reuse of the cursor. That way we don't leak tmBinds all over the place
 * when a cursor is closed and reused.
 */
void tmCursor::cleanUp() {
    for (map<unsigned, tmBind *>::iterator i = mBinds.begin(); i != mBinds.end(); ++i) {
        cerr << *(i->second);
        mBinds.erase(i);
        delete i->second;
    }
}


/** @brief Allows a tmCursor to be streamed to an ostream.
 *
 * @return	ostream & (reference).
 *
 * This function/operator allows a tmCursor to stream itself
 * out to an ostream. It does not, at present, stream out
 * a cursor's binds.
 *
 * Only used for debugging and verbose output.
 */
ostream &operator<<(ostream &out, const tmCursor &cursor) {
    out << "CursorID: " << cursor.mCursorId << endl;
    out << "SQL Line Number: " << cursor.mSQLLineNumber << endl;
    out << "SQL Text Length: " << cursor.mSQLSize << endl;
    out << "SQL Parse Line: " << cursor.mSQLParseLine << endl;
    out << "SQL Text = [" << cursor.mSQLText << "]" << endl << endl;
    out << "Bind Count: " << cursor.mBindCount << endl << endl;
    return out;
}


/** @brief Updates the SQL statement & binds when the SQL changes.
 *
 * @param std::string val. The (new) SQL statement for this tmCursor.
 *
 * When a cursor gets a new SQL statement, the binds are extracted
 * and a tmBind map set up where the key is the bind name (:xxx) and
 * the rest is the bind stuff itself.
 *
 * If a statement uses the same bind more than once, that's acceptable
 * as the bind map is keyed on the bind number not the name.
 */
void tmCursor::SetSQLText(string val) {

    // Assign the (new) SQL statement.
    mSQLText = val;

    // Build the binds list.
    buildBindMap(val);
}


/** @brief Initialises the list of Bind objects when the SQL changes.
 *
 * @return bool.
 *
 * This function does the hard work of extracting the bind variables
 * when a cursor has a (new) SQL statement assigned. Old ones are removed
 * and cleaned up.
 *
 * A return of true indicates success, false otherwise.
 */
bool tmCursor::buildBindMap(const string &sql) {

    // If we have any binds, clean them out.
    if (mBinds.size()) {
        cleanUp();
    }

    // Now, hunt down and extract any binds.
    // Beware, ":=" doesn't constitute a bind variable!
    string thisSQL = sql;
    string bindName;

    // Binds are a ":" followed by one or more underscors,
    // letters and/or digits. Double quotes optional.
    // This regex finds valid binds.
    regex reg("(:\"?\\w+\"?)");
    smatch match;

    // Oracle numbers binds from 0, in the order that
    // they appear in the SQL.
    unsigned bindID = 0;

    // Scan the SQL looking for binds.
    while (regex_search(thisSQL, match, reg)) {
        // extract the bind name, including the colon.
        bindName = match[1];

        // Save the Bind details.
        tmBind *thisBind = new tmBind(bindID, bindName);

        // An iterator for the insert into the bind map. AKA where are we?
        pair<map<unsigned, tmBind *>::iterator, bool> exists;

        // Stash this new bind. If the bind exists, update it. Maps are weird!
        exists = mBinds.insert(pair<unsigned, tmBind *>(bindID, thisBind));

        // If the bool is false, the insert failed - already there.
        // So we simply barf!.
        //
        // Exists is a pair<mBinds::iterator, bool>. It indexes the map at the
        // position of the inserted or already existing cursor data.
        // Exists.second is the bool. True=inserted, False=already exists.
        // Exists.first is an iterator (pseudo pointer) to a pair <string, tmBind *>.
        // Exists.first->first is the string, aka the bindName.
        // Exists.first->second is the tmBind pointer.
        //
        // Phew!

        // So, after all that, did we insert or find our bind?
        if (!exists.second) {
            // This should never happen!
            cerr << "Already exists: " << bindName << " with bind id "
                 << bindID << endl;
            delete thisBind;
            return false;
        }

        // Search the rest of the SQL text next time around.
        thisSQL = match.suffix();
        bindID++;
    }

    mBindCount = bindID;

    // Looking good!
    return true;
}
