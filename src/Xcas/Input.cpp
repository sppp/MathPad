#include "Xcas.h"


namespace Upp {
void RegisterCSyntax(const char *id, int kind, const char *exts, const char *description);
}


INITBLOCK {
	RegisterCSyntax("xwd", CSyntax::HIGHLIGHT_CPP, "*.xwd", "Xcas document");
	
	EditorSyntax::Register("Xcas", callback(::Xcas::CreateXcasSyntax), "*.xwd", "Xcas");
}


namespace Xcas {

void CreateXcasSyntax(One<EditorSyntax>& e) {
	e.Create<XcasSyntax>();
}

XcasSyntax::XcasSyntax() {
	
}

void XcasSyntax::Highlight(const wchar *start, const wchar *end, HighlightOutput& hls, CodeEditor *editor, int line, int pos) {
	InitKeywords();
	
	bool isComment = false;
	bool isStr = false;
	char strOpening;
	
	const wchar* p = start;
	while(p < end) {
		if((*p == '#' || isComment) && !isStr) {
			isComment = true;
			hls.Put(hl_style[INK_COMMENT]);
		}
		else
		if(*p == '\'' || *p == '\"' || isStr) {
			hls.Put(hl_style[INK_CONST_STRING]);
			if((*p == '\'' || *p == '\"') && p - 1 != start && *(p - 1) != '\\')
				if (!isStr || strOpening == *p) {
					isStr = !isStr;
					strOpening = (char)*p;
				}
		}
		else
		if(IsSeparator(p) || p == start) {
			WString w;
			bool isW = false;
			const wchar* bp = (p == start && !IsSeparator(p)) ? p : p + 1;
			while (bp != end && !IsSeparator(bp))
				w += *bp++;
			
			bool isPutted = false;
			if(IsSeparator(p)) {
				hls.Put(hl_style[INK_NORMAL]);
				isPutted = true;
			}
			if(IsKeyword(w)) {
				hls.Put(w.GetLength(), hl_style[INK_KEYWORD]);
				isW = true;
			}
			else
			if(IsSpecialVar(w)) {
				hls.Put(w.GetLength(), hl_style[INK_UPP]);
				isW = true;
			}
			else
			if(IsNumber(w)) {
				hls.Put(w.GetLength(), hl_style[INK_CONST_INT]);
				isW = true;
			}
			
			if(isW) {
				p += w.GetLength() - (isPutted ? 0 : 1);
			}
		}
		else
			hls.Put(hl_style[INK_NORMAL]);
		
		p++;
	}
}

void XcasSyntax::IndentInsert(CodeEditor& editor, int chr, int count)
{
	if(chr == '\n') {
		while(count--) {
			WString cursorLine = editor.GetWLine(editor.GetCursorLine());
			editor.InsertChar('\n', 1);
			
			Identation::Type idType = FindIdentationType(editor, cursorLine);
			char idChar = GetIdentationByType(idType);
			int mult = 1;
			if(idType == Identation::Space)
				mult = CalculateSpaceIndetationSize(editor);
			if(LineHasColon(cursorLine))
				editor.InsertChar(idChar, mult);
			editor.InsertChar(idChar, CalculateLineIndetations(cursorLine, idType));
		}
	}
	if(count > 0)
		editor.InsertChar(chr, count);
}

bool XcasSyntax::IsSeparator(const wchar* c)
{
	return    *c == ' ' || *c == ':' || *c == ',' || *c == '.' || *c == '('
	       || *c == ')' || *c == '[' || *c == ']' || *c == '{' || *c == '}'
	       || *c == '\t';
}

bool XcasSyntax::IsKeyword(const WString& w)
{
	return keywords.Find(w.ToString()) > -1;
}

bool XcasSyntax::IsSpecialVar(const WString& w)
{
	return specialVars.Find(w.ToString()) > -1;
}

bool XcasSyntax::IsNumber(const WString& w)
{
	RegExp exp("^-?[0-9]+$");
	return exp.Match(w.ToString());
}

void XcasSyntax::InitKeywords()
{
	static const char* pythonKeywords[] = {
		"and", "as", "assert", "break", "class", "continue", "def",
		"del", "elif", "else", "except", "finally", "for",
		"from", "global", "if", "import", "in", "is", "lambda",
		"not", "or", "pass", "raise", "return", "try",
		"while", "with", "yield",
		"None", "True", "False",
		NULL
	};
	static const char* pythonSpecialVars[] = {
		"self", "NotImplemented", "Ellipsis", "__debug__", "__file__", "__name__",
		NULL
	};
	
	LoadSyntax(pythonKeywords, pythonSpecialVars);
}

void XcasSyntax::LoadSyntax(const char* keywordsArray[], const char* specialVarsArray[])
{
	keywords.Clear();
	while(*keywordsArray)
		keywords.Add(*keywordsArray++);
	
	specialVars.Clear();
	while(*specialVarsArray)
		specialVars.Add(*specialVarsArray++);
}

bool XcasSyntax::LineHasColon(const WString& line)
{
	for(int i = line.GetLength() - 1; i >= 0; i--) {
		if(line[i] == ':')
			return true;
	}
	return false;
}

int XcasSyntax::CalculateLineIndetations(const WString& line, Identation::Type type)
{
	int count = 0;
	for(int i = 0; i < line.GetLength(); i++) {
		if(type == Identation::Tab && line[i] == '\t')
			count++;
		else
		if(type == Identation::Space && line[i] == ' ')
			count++;
		else
			break;
	}
	return count;
}

XcasSyntax::Identation::Type XcasSyntax::FindIdentationType(CodeEditor& editor, const WString& line)
{
	Identation::Type type = Identation::Unknown;
	if(line.StartsWith("\t"))
		type = Identation::Tab;
	else
	if(line.StartsWith(" "))
		type = Identation::Space;
	else {
		for(int i = 0; i < editor.GetLineCount(); i++) {
			WString cLine = editor.GetWLine(i);
			if(cLine.StartsWith("\t")) {
				type = Identation::Tab;
				break;
			}
			else
			if(cLine.StartsWith(" ")) {
				type = Identation::Space;
				break;
			}
		}
	}
	return type;
}

int XcasSyntax::CalculateSpaceIndetationSize(CodeEditor& editor)
{
	int current = 0;
	for(int i = 0; i < editor.GetLineCount(); i++) {
		WString line = editor.GetWLine(i);
		for(int j = 0; j < line.GetLength(); j++) {
			if(line[j] == ' ')
				current++;
			else
				break;
		}
		
		if(current > 0)
			break;
	}
	
	// TODO: 4 is magic numer - try to find the way to get this number from ide constants
	return current > 0 ? current : 4;
}

char XcasSyntax::GetIdentationByType(Identation::Type type)
{
	if(type == Identation::Space)
		return ' ';
	return '\t';
}














Input::Input() {
	
}

bool Input::Key(dword key, int count) {
	
	if (key == K_ENTER) {
		WhenExecute();
		return true;
	}
	else if (key == K_SHIFT_ENTER) {
		WhenNewline();
		return CodeEditor::Key(K_ENTER, count);
	}
	return CodeEditor::Key(key, count);
}

}
