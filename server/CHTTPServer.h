#ifndef CHTTPSERVER_H
#define CHTTPSERVER_H

#include "CString.h"
#include "BasicTypes.h"
#include "stdvector.h"

class CServer;
class CConfig;
class CHTTPRequest;
class CHTTPReply;
class IDataSocket;

class CHTTPServer {
public:
	CHTTPServer(CServer*);
	virtual ~CHTTPServer();

	// manipulators

	// synchronously process an HTTP request on the given socket
	void				processRequest(IDataSocket*);

	// accessors

protected:
	virtual void		doProcessRequest(CHTTPRequest&, CHTTPReply&);

	virtual void		doProcessGetEditMap(CHTTPRequest&, CHTTPReply&);
	virtual void		doProcessPostEditMap(CHTTPRequest&, CHTTPReply&);

	static bool			parseXY(const CString&, SInt32& x, SInt32& y);

	class CScreenArray {
	public:
		CScreenArray();
		~CScreenArray();

		// resize the array.  this also clears all the elements.
		void			resize(SInt32 w, SInt32 h);

		// insert/remove a row/column.  all elements in a new row/column
		// are unset.
		void			insertRow(SInt32 insertedBeforeRow);
		void			insertColumn(SInt32 insertedBeforeColumn);
		void			eraseRow(SInt32 row);
		void			eraseColumn(SInt32 column);

		// rotate rows or columns
		void			rotateRows(SInt32 rowsDown);
		void			rotateColumns(SInt32 columnsDown);

		// remove/set a screen name.  setting an empty name is the
		// same as removing a name.  names are not checked for
		// validity.
		void			remove(SInt32 x, SInt32 y);
		void			set(SInt32 x, SInt32 y, const CString&);

		// convert a CConfig to a CScreenArray.  returns true iff
		// all connections are symmetric and therefore exactly
		// representable by a CScreenArray.
		bool			convertFrom(const CConfig&);

		// accessors

		// get the array size
		SInt32			getWidth() const  { return m_w; }
		SInt32			getHeight() const { return m_h; }

		// returns true iff the cell has a 4-connected neighbor
		bool			isAllowed(SInt32 x, SInt32 y) const;

		// returns true iff the cell has a (non-empty) name
		bool			isSet(SInt32 x, SInt32 y) const;

		// get a screen name
		CString			get(SInt32 x, SInt32 y) const;

		// find a screen by name.  returns true iff found.
		bool			find(const CString&, SInt32& x, SInt32& y) const;

		// return true iff the overall array is valid.  that means
		// just zero or one screen or all screens are 4-connected
		// to other screens.
		bool			isValid() const;

		// convert this to a CConfig
		void			convertTo(CConfig&) const;

	private:
		typedef std::vector<CString> CNames;

		SInt32			m_w, m_h;
		CNames			m_screens;
	};

private:
	CServer*			m_server;
	static const UInt32	s_maxRequestSize;
};

#endif
