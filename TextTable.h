#pragma once

#include <iostream>
#include <map>
#include <iomanip>
#include <vector>
#include <string>


using namespace std;

#ifdef TEXTTABLE_ENCODE_MULTIBYTE_STRINGS
#include <clocale>
#ifndef TEXTTABLE_USE_EN_US_UTF8
#define TEXTTABLE_USE_EN_US_UTF8
#endif
#endif

class TextTable {

    public:
    enum class Alignment { LEFT, RIGHT }; 
    typedef std::vector< std::string > Row;
    TextTable() :
        _horizontal( '-' ),
        _vertical( '|' ),
        _corner( '+' ),
		_has_ruler(true)
    {}

    TextTable( char horizontal, char vertical, char corner ) :
        _horizontal( horizontal ),
        _vertical( vertical ),
        _corner( corner ),
		_has_ruler(true)
    {}
    
    explicit TextTable( char vertical ) :
        _horizontal( '\0' ),
        _vertical( vertical ),
        _corner( '\0' ),
		_has_ruler( false )
    {}

    void setAlignment( unsigned i, Alignment alignment )
    {
        _alignment[ i ] = alignment;
    }

    Alignment alignment( unsigned i ) const
    { return _alignment[ i ]; }

    char vertical() const
    { return _vertical; }

    char horizontal() const
    { return _horizontal; }

    void add( std::string const & content )
    {
        _current.push_back( content );
    }

    void endOfRow()
    {
        _rows.push_back( _current );
        _current.assign( 0, "" );
    }

    template <typename Iterator>
    void addRow( Iterator begin, Iterator end )
    {
        for( auto i = begin; i != end; ++i ) {
           add( * i ); 
        }
        endOfRow();
    }

    template <typename Container>
    void addRow( Container const & container )
    {
        addRow( container.begin(), container.end() );
    }

    std::vector< Row > const & rows() const
    {
        return _rows;
    }

    void setup() const
    {
        determineWidths();
        setupAlignment();
    }

    std::string ruler() const
    {
        std::string result;
        result += _corner;
        for( auto width = _width.begin(); width != _width.end(); ++ width ) {
            result += repeat( * width, _horizontal );
            result += _corner;
        }

        return result;
    }

    int width( unsigned i ) const
    { return _width[ i ]; }

	bool has_ruler() const { return _has_ruler;}

	int correctDistance(std::string string_to_correct) const
		{
			return static_cast<int>(string_to_correct.size()) - static_cast<int>(glyphLength(string_to_correct));
		};
	
    private:
    const char _horizontal;
    const char _vertical;
    const char _corner;
    const bool _has_ruler;
    Row _current;
    std::vector< Row > _rows;
    std::vector< unsigned > mutable _width;
	std::vector< unsigned > mutable _utf8width;
    std::map< unsigned, Alignment > mutable _alignment;
	
    static std::string repeat( unsigned times, char c )
    {
        std::string result;
        for( ; times > 0; -- times )
            result += c;

        return result;
    }

    unsigned columns() const
    {
        return _rows[ 0 ].size();
    }

	unsigned glyphLength( std::string s ) const
	{
		unsigned int _byteLength = s.length();
#ifdef TEXTTABLE_ENCODE_MULTIBYTE_STRINGS
#ifdef TEXTTABLE_USE_EN_US_UTF8
		std::setlocale(LC_ALL, "en_US.utf8");
#else
#error You need to specify the encoding if the TextTable library uses multybyte string encoding!
#endif
		unsigned int u = 0;
		const char *c_str = s.c_str();
		unsigned _glyphLength = 0;
		while(u < _byteLength)
		{
			u += std::mblen(&c_str[u], _byteLength - u);
			_glyphLength += 1;
		}
		return _glyphLength;
#else
		return _byteLength;
#endif
	}
	
    void determineWidths() const
    {
        _width.assign( columns(), 0 );
		_utf8width.assign( columns(), 0 );
        for ( auto rowIterator = _rows.begin(); rowIterator != _rows.end(); ++ rowIterator ) {
            Row const & row = * rowIterator;
            for ( unsigned i = 0; i < row.size(); ++i ) {
                _width[ i ] = _width[ i ] > glyphLength(row[ i ]) ? _width[ i ] : glyphLength(row[ i ]);
            }
        }
    }

    void setupAlignment() const
    {
        for ( unsigned i = 0; i < columns(); ++i ) {
            if ( _alignment.find( i ) == _alignment.end() ) {
                _alignment[ i ] = Alignment::LEFT;
            }
        }
    }
};

class TableVendorList {

public:
    enum class Alignment { LEFT, RIGHT };
    typedef std::vector< std::string > Row;
    TableVendorList() :
        _horizontal('-'),
        _vertical('|'),
        _corner('+'),
        _has_ruler(true)
    {}

    TableVendorList(char horizontal, char vertical, char corner) :
        _horizontal(horizontal),
        _vertical(vertical),
        _corner(corner),
        _has_ruler(true)
    {}

    explicit TableVendorList(char vertical) :
        _horizontal('\0'),
        _vertical(vertical),
        _corner('\0'),
        _has_ruler(false)
    {}

    void setAlignment(unsigned i, Alignment alignment)
    {
        _alignment[i] = alignment;
    }

    Alignment alignment(unsigned i) const
    {
        return _alignment[i];
    }

    char vertical() const
    {
        return _vertical;
    }

    char horizontal() const
    {
        return _horizontal;
    }

    void add(std::string const& content)
    {
        _current.push_back(content);
    }

    void endOfRow()
    {
        _rows.push_back(_current);
        _current.assign(0, "");
    }

    template <typename Iterator>
    void addRow(Iterator begin, Iterator end)
    {
        for (auto i = begin; i != end; ++i) {
            add(*i);
        }
        endOfRow();
    }

    template <typename Container>
    void addRow(Container const& container)
    {
        addRow(container.begin(), container.end());
    }

    std::vector< Row > const& rows() const
    {
        return _rows;
    }

    void setup() const
    {
        determineWidths();
        setupAlignment();
    }

    std::string ruler() const
    {
        std::string result;
        result += _corner;
        for (auto width = _width.begin(); width != _width.end(); ++width) {
            result += repeat(*width, _horizontal);
            result += _corner;
        }

        return result;
    }

    int width(unsigned i) const
    {
        return _width[i];
    }

    bool has_ruler() const { return _has_ruler; }

    int correctDistance(std::string string_to_correct) const
    {
        return static_cast<int>(string_to_correct.size()) - static_cast<int>(glyphLength(string_to_correct));
    };

private:
    const char _horizontal;
    const char _vertical;
    const char _corner;
    const bool _has_ruler;
    Row _current;
    std::vector< Row > _rows;
    std::vector< unsigned > mutable _width;
    std::vector< unsigned > mutable _utf8width;
    std::map< unsigned, Alignment > mutable _alignment;

    static std::string repeat(unsigned times, char c)
    {
        std::string result;
        for (; times > 0; --times)
            result += c;

        return result;
    }

    unsigned columns() const
    {
        return _rows[0].size();
    }

    unsigned glyphLength(std::string s) const
    {
        unsigned int _byteLength = s.length();
#ifdef TEXTTABLE_ENCODE_MULTIBYTE_STRINGS
#ifdef TEXTTABLE_USE_EN_US_UTF8
        std::setlocale(LC_ALL, "en_US.utf8");
#else
#error You need to specify the encoding if the TextTable library uses multybyte string encoding!
#endif
        unsigned int u = 0;
        const char* c_str = s.c_str();
        unsigned _glyphLength = 0;
        while (u < _byteLength)
        {
            u += std::mblen(&c_str[u], _byteLength - u);
            _glyphLength += 1;
        }
        return _glyphLength;
#else
        return _byteLength;
#endif
    }

    void determineWidths() const
    {
        _width.assign(columns(), 0);
        _utf8width.assign(columns(), 0);
        for (auto rowIterator = _rows.begin(); rowIterator != _rows.end(); ++rowIterator) {
            Row const& row = *rowIterator;
            for (unsigned i = 0; i < row.size(); ++i) {
                _width[i] = _width[i] > glyphLength(row[i]) ? _width[i] : glyphLength(row[i]);
            }
        }
    }

    void setupAlignment() const
    {
        for (unsigned i = 0; i < columns(); ++i) {
            if (_alignment.find(i) == _alignment.end()) {
                _alignment[i] = Alignment::LEFT;
            }
        }
    }
};

class TableProduct {

public:
    enum class Alignment { LEFT, RIGHT };
    typedef std::vector< std::string > Row;
    TableProduct() :
        _horizontal('-'),
        _vertical('|'),
        _corner('+'),
        _has_ruler(true)
    {}

    TableProduct(char horizontal, char vertical, char corner) :
        _horizontal(horizontal),
        _vertical(vertical),
        _corner(corner),
        _has_ruler(true)
    {}

    explicit TableProduct(char vertical) :
        _horizontal('\0'),
        _vertical(vertical),
        _corner('\0'),
        _has_ruler(false)
    {}

    void setAlignment(unsigned i, Alignment alignment)
    {
        _alignment[i] = alignment;
    }

    Alignment alignment(unsigned i) const
    {
        return _alignment[i];
    }

    char vertical() const
    {
        return _vertical;
    }

    char horizontal() const
    {
        return _horizontal;
    }

    void add(std::string const& content)
    {
        _current.push_back(content);
    }

    void endOfRow()
    {
        _rows.push_back(_current);
        _current.assign(0, "");
    }

    template <typename Iterator>
    void addRow(Iterator begin, Iterator end)
    {
        for (auto i = begin; i != end; ++i) {
            add(*i);
        }
        endOfRow();
    }

    template <typename Container>
    void addRow(Container const& container)
    {
        addRow(container.begin(), container.end());
    }

    std::vector< Row > const& rows() const
    {
        return _rows;
    }

    void setup() const
    {
        determineWidths();
        setupAlignment();
    }

    std::string ruler() const
    {
        std::string result;
        result += _corner;
        for (auto width = _width.begin(); width != _width.end(); ++width) {
            result += repeat(*width, _horizontal);
            result += _corner;
        }

        return result;
    }

    int width(unsigned i) const
    {
        return _width[i];
    }

    bool has_ruler() const { return _has_ruler; }

    int correctDistance(std::string string_to_correct) const
    {
        return static_cast<int>(string_to_correct.size()) - static_cast<int>(glyphLength(string_to_correct));
    };

private:
    const char _horizontal;
    const char _vertical;
    const char _corner;
    const bool _has_ruler;
    Row _current;
    std::vector< Row > _rows;
    std::vector< unsigned > mutable _width;
    std::vector< unsigned > mutable _utf8width;
    std::map< unsigned, Alignment > mutable _alignment;

    static std::string repeat(unsigned times, char c)
    {
        std::string result;
        for (; times > 0; --times)
            result += c;

        return result;
    }

    unsigned columns() const
    {
        return _rows[0].size();
    }

    unsigned glyphLength(std::string s) const
    {
        unsigned int _byteLength = s.length();
#ifdef TEXTTABLE_ENCODE_MULTIBYTE_STRINGS
#ifdef TEXTTABLE_USE_EN_US_UTF8
        std::setlocale(LC_ALL, "en_US.utf8");
#else
#error You need to specify the encoding if the TextTable library uses multybyte string encoding!
#endif
        unsigned int u = 0;
        const char* c_str = s.c_str();
        unsigned _glyphLength = 0;
        while (u < _byteLength)
        {
            u += std::mblen(&c_str[u], _byteLength - u);
            _glyphLength += 1;
        }
        return _glyphLength;
#else
        return _byteLength;
#endif
    }

    void determineWidths() const
    {
        _width.assign(columns(), 0);
        _utf8width.assign(columns(), 0);
        for (auto rowIterator = _rows.begin(); rowIterator != _rows.end(); ++rowIterator) {
            Row const& row = *rowIterator;
            for (unsigned i = 0; i < row.size(); ++i) {
                _width[i] = _width[i] > glyphLength(row[i]) ? _width[i] : glyphLength(row[i]);
            }
        }
    }

    void setupAlignment() const
    {
        for (unsigned i = 0; i < columns(); ++i) {
            if (_alignment.find(i) == _alignment.end()) {
                _alignment[i] = Alignment::LEFT;
            }
        }
    }
};

class TableOrder {

public:
    enum class Alignment { LEFT, RIGHT };
    typedef std::vector< std::string > Row;
    TableOrder() :
        _horizontal('-'),
        _vertical('|'),
        _corner('+'),
        _has_ruler(true)
    {}

    TableOrder(char horizontal, char vertical, char corner) :
        _horizontal(horizontal),
        _vertical(vertical),
        _corner(corner),
        _has_ruler(true)
    {}

    explicit TableOrder(char vertical) :
        _horizontal('\0'),
        _vertical(vertical),
        _corner('\0'),
        _has_ruler(false)
    {}

    void setAlignment(unsigned i, Alignment alignment)
    {
        _alignment[i] = alignment;
    }

    Alignment alignment(unsigned i) const
    {
        return _alignment[i];
    }

    char vertical() const
    {
        return _vertical;
    }

    char horizontal() const
    {
        return _horizontal;
    }

    void add(std::string const& content)
    {
        _current.push_back(content);
    }

    void endOfRow()
    {
        _rows.push_back(_current);
        _current.assign(0, "");
    }

    template <typename Iterator>
    void addRow(Iterator begin, Iterator end)
    {
        for (auto i = begin; i != end; ++i) {
            add(*i);
        }
        endOfRow();
    }

    template <typename Container>
    void addRow(Container const& container)
    {
        addRow(container.begin(), container.end());
    }

    std::vector< Row > const& rows() const
    {
        return _rows;
    }

    void setup() const
    {
        determineWidths();
        setupAlignment();
    }

    std::string ruler() const
    {
        std::string result;
        result += _corner;
        for (auto width = _width.begin(); width != _width.end(); ++width) {
            result += repeat(*width, _horizontal);
            result += _corner;
        }

        return result;
    }

    int width(unsigned i) const
    {
        return _width[i];
    }

    bool has_ruler() const { return _has_ruler; }

    int correctDistance(std::string string_to_correct) const
    {
        return static_cast<int>(string_to_correct.size()) - static_cast<int>(glyphLength(string_to_correct));
    };

private:
    const char _horizontal;
    const char _vertical;
    const char _corner;
    const bool _has_ruler;
    Row _current;
    std::vector< Row > _rows;
    std::vector< unsigned > mutable _width;
    std::vector< unsigned > mutable _utf8width;
    std::map< unsigned, Alignment > mutable _alignment;

    static std::string repeat(unsigned times, char c)
    {
        std::string result;
        for (; times > 0; --times)
            result += c;

        return result;
    }

    unsigned columns() const
    {
        return _rows[0].size();
    }

    unsigned glyphLength(std::string s) const
    {
        unsigned int _byteLength = s.length();
#ifdef TEXTTABLE_ENCODE_MULTIBYTE_STRINGS
#ifdef TEXTTABLE_USE_EN_US_UTF8
        std::setlocale(LC_ALL, "en_US.utf8");
#else
#error You need to specify the encoding if the TextTable library uses multybyte string encoding!
#endif
        unsigned int u = 0;
        const char* c_str = s.c_str();
        unsigned _glyphLength = 0;
        while (u < _byteLength)
        {
            u += std::mblen(&c_str[u], _byteLength - u);
            _glyphLength += 1;
        }
        return _glyphLength;
#else
        return _byteLength;
#endif
    }

    void determineWidths() const
    {
        _width.assign(columns(), 0);
        _utf8width.assign(columns(), 0);
        for (auto rowIterator = _rows.begin(); rowIterator != _rows.end(); ++rowIterator) {
            Row const& row = *rowIterator;
            for (unsigned i = 0; i < row.size(); ++i) {
                _width[i] = _width[i] > glyphLength(row[i]) ? _width[i] : glyphLength(row[i]);
            }
        }
    }

    void setupAlignment() const
    {
        for (unsigned i = 0; i < columns(); ++i) {
            if (_alignment.find(i) == _alignment.end()) {
                _alignment[i] = Alignment::LEFT;
            }
        }
    }
};

class TableCart {

public:
    enum class Alignment { LEFT, RIGHT };
    typedef std::vector< std::string > Row;
    TableCart() :
        _horizontal('-'),
        _vertical('|'),
        _corner('+'),
        _has_ruler(true)
    {}

    TableCart(char horizontal, char vertical, char corner) :
        _horizontal(horizontal),
        _vertical(vertical),
        _corner(corner),
        _has_ruler(true)
    {}

    explicit TableCart(char vertical) :
        _horizontal('\0'),
        _vertical(vertical),
        _corner('\0'),
        _has_ruler(false)
    {}

    void setAlignment(unsigned i, Alignment alignment)
    {
        _alignment[i] = alignment;
    }

    Alignment alignment(unsigned i) const
    {
        return _alignment[i];
    }

    char vertical() const
    {
        return _vertical;
    }

    char horizontal() const
    {
        return _horizontal;
    }

    void add(std::string const& content)
    {
        _current.push_back(content);
    }

    void endOfRow()
    {
        _rows.push_back(_current);
        _current.assign(0, "");
    }

    template <typename Iterator>
    void addRow(Iterator begin, Iterator end)
    {
        for (auto i = begin; i != end; ++i) {
            add(*i);
        }
        endOfRow();
    }

    template <typename Container>
    void addRow(Container const& container)
    {
        addRow(container.begin(), container.end());
    }

    std::vector< Row > const& rows() const
    {
        return _rows;
    }

    void setup() const
    {
        determineWidths();
        setupAlignment();
    }

    std::string ruler() const
    {
        std::string result;
        result += _corner;
        for (auto width = _width.begin(); width != _width.end(); ++width) {
            result += repeat(*width, _horizontal);
            result += _corner;
        }

        return result;
    }

    int width(unsigned i) const
    {
        return _width[i];
    }

    bool has_ruler() const { return _has_ruler; }

    int correctDistance(std::string string_to_correct) const
    {
        return static_cast<int>(string_to_correct.size()) - static_cast<int>(glyphLength(string_to_correct));
    };

private:
    const char _horizontal;
    const char _vertical;
    const char _corner;
    const bool _has_ruler;
    Row _current;
    std::vector< Row > _rows;
    std::vector< unsigned > mutable _width;
    std::vector< unsigned > mutable _utf8width;
    std::map< unsigned, Alignment > mutable _alignment;

    static std::string repeat(unsigned times, char c)
    {
        std::string result;
        for (; times > 0; --times)
            result += c;

        return result;
    }

    unsigned columns() const
    {
        return _rows[0].size();
    }

    unsigned glyphLength(std::string s) const
    {
        unsigned int _byteLength = s.length();
#ifdef TEXTTABLE_ENCODE_MULTIBYTE_STRINGS
#ifdef TEXTTABLE_USE_EN_US_UTF8
        std::setlocale(LC_ALL, "en_US.utf8");
#else
#error You need to specify the encoding if the TextTable library uses multybyte string encoding!
#endif
        unsigned int u = 0;
        const char* c_str = s.c_str();
        unsigned _glyphLength = 0;
        while (u < _byteLength)
        {
            u += std::mblen(&c_str[u], _byteLength - u);
            _glyphLength += 1;
        }
        return _glyphLength;
#else
        return _byteLength;
#endif
    }

    void determineWidths() const
    {
        _width.assign(columns(), 0);
        _utf8width.assign(columns(), 0);
        for (auto rowIterator = _rows.begin(); rowIterator != _rows.end(); ++rowIterator) {
            Row const& row = *rowIterator;
            for (unsigned i = 0; i < row.size(); ++i) {
                _width[i] = _width[i] > glyphLength(row[i]) ? _width[i] : glyphLength(row[i]);
            }
        }
    }

    void setupAlignment() const
    {
        for (unsigned i = 0; i < columns(); ++i) {
            if (_alignment.find(i) == _alignment.end()) {
                _alignment[i] = Alignment::LEFT;
            }
        }
    }
};

// 60
inline std::ostream & operator<<( std::ostream & stream, TextTable const & table )
{
    table.setup();
	if (table.has_ruler()) {
        //cout << "1";

        //std::cout << "               "; // tambah
        //int padding_left = (150 / 2) - (column_widths.size() / 2);
        // Put padding spaces
        int padding_left = 60;
        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
	    stream << table.ruler() << "\n";
	}
    for ( auto rowIterator = table.rows().begin(); rowIterator != table.rows().end(); ++ rowIterator ) {
        TextTable::Row const & row = * rowIterator;
        //cout << "2";

        int padding_left = 60;
        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        stream << table.vertical();
        for ( unsigned i = 0; i < row.size(); ++i ) {
            auto alignment = table.alignment( i ) == TextTable::Alignment::LEFT ? std::left : std::right;
			// std::setw( width ) works as follows: a string which goes in the stream with byte length (!) l is filled with n spaces so that l+n=width.
			// For a utf8 encoded string the glyph length g might be smaller than l. We need n spaces so that g+n=width which is equivalent to g+n+l-l=width ==> l+n = width+l-g
			// l-g (that means glyph length minus byte length) has to be added to the width argument.
			// l-g is computed by correctDistance.
           
            //cout << "3";

            stream << std::setw( table.width( i ) + table.correctDistance(row[ i ])) << alignment << row[ i ];
            
            //cout << "4";

            stream << table.vertical();

           // cout << "5";
        }

        //cout << "6";

        stream << "\n";

        //cout << "7";

		if (table.has_ruler()) {

       // cout << "8";
        int padding_left = 60;
        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        	stream << table.ruler() << "\n";
		}
    }

    return stream;
}

// 80
inline std::ostream& operator<<(std::ostream& stream, TableVendorList const& table)
{
    int padding_left = 80;

    table.setup();
    if (table.has_ruler()) {
        //cout << "1";

        //std::cout << "               "; // tambah
        //int padding_left = (150 / 2) - (column_widths.size() / 2);
        // Put padding spaces
        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        stream << table.ruler() << "\n";
    }
    for (auto rowIterator = table.rows().begin(); rowIterator != table.rows().end(); ++rowIterator) {
        TableVendorList::Row const& row = *rowIterator;
        //cout << "2";

        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        stream << table.vertical();
        for (unsigned i = 0; i < row.size(); ++i) {
            auto alignment = table.alignment(i) == TableVendorList::Alignment::LEFT ? std::left : std::right;
            // std::setw( width ) works as follows: a string which goes in the stream with byte length (!) l is filled with n spaces so that l+n=width.
            // For a utf8 encoded string the glyph length g might be smaller than l. We need n spaces so that g+n=width which is equivalent to g+n+l-l=width ==> l+n = width+l-g
            // l-g (that means glyph length minus byte length) has to be added to the width argument.
            // l-g is computed by correctDistance.

            //cout << "3";

            stream << std::setw(table.width(i) + table.correctDistance(row[i])) << alignment << row[i];

            //cout << "4";

            stream << table.vertical();

            // cout << "5";
        }

        //cout << "6";

        stream << "\n";

        //cout << "7";

        if (table.has_ruler()) {

            // cout << "8";
            for (int i = 0; i < padding_left; ++i)
                std::cout << ' ';
            stream << table.ruler() << "\n";
        }
    }

    return stream;
}

// 83
inline std::ostream& operator<<(std::ostream& stream, TableProduct const& table)
{
    int padding_left = 83;

    table.setup();
    if (table.has_ruler()) {
        //cout << "1";

        //std::cout << "               "; // tambah
        //int padding_left = (150 / 2) - (column_widths.size() / 2);
        // Put padding spaces
        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        stream << table.ruler() << "\n";
    }
    for (auto rowIterator = table.rows().begin(); rowIterator != table.rows().end(); ++rowIterator) {
        TableOrder::Row const& row = *rowIterator;
        //cout << "2";

        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        stream << table.vertical();
        for (unsigned i = 0; i < row.size(); ++i) {
            auto alignment = table.alignment(i) == TableProduct::Alignment::LEFT ? std::left : std::right;
            // std::setw( width ) works as follows: a string which goes in the stream with byte length (!) l is filled with n spaces so that l+n=width.
            // For a utf8 encoded string the glyph length g might be smaller than l. We need n spaces so that g+n=width which is equivalent to g+n+l-l=width ==> l+n = width+l-g
            // l-g (that means glyph length minus byte length) has to be added to the width argument.
            // l-g is computed by correctDistance.

            //cout << "3";

            stream << std::setw(table.width(i) + table.correctDistance(row[i])) << alignment << row[i];

            //cout << "4";

            stream << table.vertical();

            // cout << "5";
        }

        //cout << "6";

        stream << "\n";

        //cout << "7";

        if (table.has_ruler()) {

            // cout << "8";
            for (int i = 0; i < padding_left; ++i)
                std::cout << ' ';
            stream << table.ruler() << "\n";
        }
    }

    return stream;
}

// 69
inline std::ostream& operator<<(std::ostream& stream, TableOrder const& table)
{
    int padding_left = 69;

    table.setup();
    if (table.has_ruler()) {
        //cout << "1";

        //std::cout << "               "; // tambah
        //int padding_left = (150 / 2) - (column_widths.size() / 2);
        // Put padding spaces
        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        stream << table.ruler() << "\n";
    }
    for (auto rowIterator = table.rows().begin(); rowIterator != table.rows().end(); ++rowIterator) {
        TableOrder::Row const& row = *rowIterator;
        //cout << "2";

        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        stream << table.vertical();
        for (unsigned i = 0; i < row.size(); ++i) {
            auto alignment = table.alignment(i) == TableOrder::Alignment::LEFT ? std::left : std::right;
            // std::setw( width ) works as follows: a string which goes in the stream with byte length (!) l is filled with n spaces so that l+n=width.
            // For a utf8 encoded string the glyph length g might be smaller than l. We need n spaces so that g+n=width which is equivalent to g+n+l-l=width ==> l+n = width+l-g
            // l-g (that means glyph length minus byte length) has to be added to the width argument.
            // l-g is computed by correctDistance.

            //cout << "3";

            stream << std::setw(table.width(i) + table.correctDistance(row[i])) << alignment << row[i];

            //cout << "4";

            stream << table.vertical();

            // cout << "5";
        }

        //cout << "6";

        stream << "\n";

        //cout << "7";

        if (table.has_ruler()) {

            // cout << "8";
            for (int i = 0; i < padding_left; ++i)
                std::cout << ' ';
            stream << table.ruler() << "\n";
        }
    }

    return stream;
}

// 75
inline std::ostream& operator<<(std::ostream& stream, TableCart const& table)
{
    int padding_left = 75;

    table.setup();
    if (table.has_ruler()) {
        //cout << "1";

        //std::cout << "               "; // tambah
        //int padding_left = (150 / 2) - (column_widths.size() / 2);
        // Put padding spaces
        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        stream << table.ruler() << "\n";
    }
    for (auto rowIterator = table.rows().begin(); rowIterator != table.rows().end(); ++rowIterator) {
        TableCart::Row const& row = *rowIterator;
        //cout << "2";

        for (int i = 0; i < padding_left; ++i)
            std::cout << ' ';
        stream << table.vertical();
        for (unsigned i = 0; i < row.size(); ++i) {
            auto alignment = table.alignment(i) == TableCart::Alignment::LEFT ? std::left : std::right;
            // std::setw( width ) works as follows: a string which goes in the stream with byte length (!) l is filled with n spaces so that l+n=width.
            // For a utf8 encoded string the glyph length g might be smaller than l. We need n spaces so that g+n=width which is equivalent to g+n+l-l=width ==> l+n = width+l-g
            // l-g (that means glyph length minus byte length) has to be added to the width argument.
            // l-g is computed by correctDistance.

            //cout << "3";

            stream << std::setw(table.width(i) + table.correctDistance(row[i])) << alignment << row[i];

            //cout << "4";

            stream << table.vertical();

            // cout << "5";
        }

        //cout << "6";

        stream << "\n";

        //cout << "7";

        if (table.has_ruler()) {

            // cout << "8";
            for (int i = 0; i < padding_left; ++i)
                std::cout << ' ';
            stream << table.ruler() << "\n";
        }
    }

    return stream;
}