#if 0

#ifndef _Xcas_Input_h_
#define _Xcas_Input_h_

namespace xcas {

// If use_external_browser is true you can change the browser used
// by setting the BROWSER env variable, e.g. export BROWSER=...
extern bool use_external_browser;
// alert if the command is not successfull
void system_browser(const std::string & s);

// return the last keyword of s
std::string motclef(const std::string & s);

#ifdef HAVE_LIBFLTK

extern Fl_Help_Dialog * Xcas_help_window;
extern Fl_Window * handle_tab_w ;

// Default vector of completions inside the Multiline_Input_tab
// Use the function read_aide to load completions from a help file
bool read_aide(const std::string & helpfilename, int language);
// Help window for the application, return 0 if cancel, 1 if add () or 2 if no ()
int handle_tab(const std::string & s, const std::vector<std::string> & v, int dx, int dy, int & remove, std::string & ans, bool allow_immediate_out = true);

int height(const char *, int);

class Flv_Table_Gen ;

void Multiline_default_callback(Fl_Widget * w, void *);

extern Fl_Widget * Xcas_input_focus;

class Multiline_Input_tab: public Fl_Multiline_Input {
	static int count;
	static std::vector<std::string> history;
	bool handling;
	int in_handle(int);
public:
	std::vector<std::string> * completion_tab ;
	virtual FL_EXPORT int handle(int);
	// virtual FL_EXPORT void draw();
	// if selected is true, it will select the insertion
	void insert_replace(const std::string & chaine, bool selected);
	Flv_Table_Gen * tableur;
	giac::gen _g; // If the widget is not changed() then parse will use g
	giac::gen g();
	void set_g(const giac::gen & g);
	void match(); // Show matching parenthesis, [ , etc. region
	Multiline_Input_tab(int x, int y, int w, int h, const char * l = 0);
	virtual ~Multiline_Input_tab() { if (Xcas_input_focus == this) Xcas_input_focus = 0;}
	void resize_nl(); // resize input and parents according to number of lines
	bool need_nl();
	// return true if current line up to position is too large to be displayed
	// with the current widget width (and font)
};

void increase_size(Fl_Widget * wid, int L); // change parents size in a History_Pack

class Gen_Output: public Fl_Multiline_Output {
public:
	giac::gen g;
	void value(const giac::gen & _g);
	void value(const char * ch);
	virtual giac::gen value() const;
	Gen_Output(int X, int Y, int W, int H, const char * l = 0): Fl_Multiline_Output(X, Y, W, H, l), g(0) { Fl_Multiline_Output::value("0");     color(FL_WHITE); }
};

class Enlargable_Multiline_Output: public Fl_Multiline_Output {
public:
	Enlargable_Multiline_Output(int x, int y, int w, int h, const char * l = 0): Fl_Multiline_Output(x, y, w, h, l) {}
	void resize();
	void value(const char * ch);
};

class Comment_Multiline_Input: public Fl_Multiline_Input {
public:
	Comment_Multiline_Input(int x, int y, int w, int h, const char * l = 0);
	virtual FL_EXPORT int handle(int);
	virtual FL_EXPORT int in_handle(int);
	virtual ~Comment_Multiline_Input() { if (Xcas_input_focus == this) Xcas_input_focus = 0;}
};

// Convert a pnt to a user-readable string
std::string pnt2string(const giac::gen & g, const giac::context * contextptr);


#endif // HAVE_LIBFLTK

#ifndef NO_NAMESPACE_XCAS
}

#endif
#endif
