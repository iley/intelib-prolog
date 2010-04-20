

#include "sexpress/iexcept.hpp"
#include "sexpress/sexpress.hpp"

#include "refal/refal.hpp"
#include "refal/rclause.hpp"
#include "refal/rvars.hpp"

static RfFormConstructor R;

RfWith RWITH;


extern RfLibSymbol Mu;
extern RfLibSymbol Add;
extern RfLibSymbol Arg;
extern RfLibSymbol Br;
extern RfLibSymbol Card;
extern RfLibSymbol Chr;
extern RfLibSymbol Cp;
extern RfLibSymbol Dg;
extern RfLibSymbol Dgall;
extern RfLibSymbol Div;
extern RfLibSymbol Explode;
extern RfLibSymbol Get;
extern RfLibSymbol Lenw;
extern RfLibSymbol Mod;
extern RfLibSymbol Mul;
extern RfLibSymbol Numb;
extern RfLibSymbol Open;
extern RfLibSymbol Ord;
extern RfLibSymbol Print;
extern RfLibSymbol Prout;
extern RfLibSymbol Put;
extern RfLibSymbol Putout;
extern RfLibSymbol Rp;
extern RfLibSymbol Sub;
extern RfLibSymbol Symb;
extern RfLibSymbol Time;
extern RfLibSymbol Type;
extern RfLibSymbol GetEnv;
extern RfLibSymbol Exit;
extern RfLibSymbol Close;
extern RfLibSymbol ExistFile;
extern RfLibSymbol GetCurrentDirectory;
extern RfLibSymbol RemoveFile;
extern RfLibSymbol Compare;
extern RfLibSymbol ListOfBuiltin;
extern RfLibSymbol GetPID;
extern RfLibSymbol GetPPID;
extern RfLibSymbol Argc;
extern RfLibSymbol Real;
extern RfLibSymbol Trunc;
extern RfLibSymbol Sin;
extern RfLibSymbol Cos;
extern RfLibSymbol Tan;
extern RfLibSymbol Asin;
extern RfLibSymbol Acos;
extern RfLibSymbol Atan;
extern RfLibSymbol Pi;
extern RfLibSymbol Sqrt;
extern RfLibSymbol Exp;
extern RfLibSymbol Log;
extern RfLibSymbol Log10;
extern RfLibSymbol Pow;
extern RfLibSymbol Compress;
extern RfLibSymbol Decompress;
extern RfLibSymbol Clear;


extern RfSymbol True;
extern RfSymbol False;
extern RfSymbol regular;
extern RfSymbol special;


static RfSymbol ErrorsList("ErrorsList");
static RfSymbol IrinaError_IllegalChar("IrinaError_IllegalChar");
static RfSymbol IrinaError_BadPragma("IrinaError_BadPragma");
static RfSymbol IrinaError_QuoteMissing("IrinaError_QuoteMissing");
static RfSymbol IrinaError_BadMultiComment("IrinaError_BadMultiComment");
static RfSymbol IrinaError_BadMinus("IrinaError_BadMinus");
static RfSymbol IrinaError_BadSuffix("IrinaError_BadSuffix");
static RfSymbol IrinaError_BadFract("IrinaError_BadFract");
static RfSymbol FileName("FileName");
static RfSymbol LinkerErrors("LinkerErrors");
static RfSymbol IrinaError_NoPairFor("IrinaError_NoPairFor");
static RfSymbol IrinaError_IllegalExpr("IrinaError_IllegalExpr");
static RfSymbol IrinaError_EntryRedefinition("IrinaError_EntryRedefinition");
static RfSymbol IrinaError_UnresolvedExternal("IrinaError_UnresolvedExternal");
static RfSymbol IrinaError_Redefinition("IrinaError_Redefinition");
static RfSymbol IrinaError_EmptyFun("IrinaError_EmptyFun");
static RfSymbol IrinaError_MatchMissing("IrinaError_MatchMissing");
static RfSymbol IrinaError_ColonMissing("IrinaError_ColonMissing");
static RfSymbol IrinaError_BadFuncall("IrinaError_BadFuncall");
static RfSymbol IrinaError_UndefinedFun("IrinaError_UndefinedFun");
static RfSymbol IrinaError_UndefinedVar("IrinaError_UndefinedVar");
static RfSymbol IrinaError_NonStandardPrefix("IrinaError_NonStandardPrefix");
static RfSymbol IrinaError_UnimplementedLibFun("IrinaError_UnimplementedLibFun");
static RfSymbol IrinaError_PublicIsNotEntry("IrinaError_PublicIsNotEntry");
static RfSymbol ExternFuns("ExternFuns");
static RfSymbol Semicolon("Semicolon");
static RfSymbol Comma("Comma");
static RfSymbol Equal("Equal");
static RfSymbol Colon("Colon");
static RfSymbol TypeEntry("TypeEntry");
static RfSymbol TypeExtern("TypeExtern");
static RfSymbol TypeDirect("TypeDirect");
static RfSymbol TypeDelim("TypeDelim");
static RfSymbol TypeText("TypeText");
static RfSymbol TypeSymbol("TypeSymbol");
static RfSymbol TypeNumber("TypeNumber");
static RfSymbol TypeVar("TypeVar");
static RfSymbol LCurBracket("LCurBracket");
static RfSymbol RCurBracket("RCurBracket");
static RfSymbol LBracket("LBracket");
static RfSymbol RBracket("RBracket");
static RfSymbol LFunBracket("LFunBracket");
static RfSymbol RFunBracket("RFunBracket");


static RfVariable_E e_Files("e.Files");
static RfVariable_T t_File("t.File");
static RfVariable_E e_Foo("e.Foo");
static RfVariable_T t_FileName("t.FileName");
static RfVariable_E e_FileCont("e.FileCont");
static RfVariable_S s_LineNumber("s.LineNumber");
static RfVariable_T t_Err("t.Err");
static RfVariable_S s_Symbol("s.Symbol");
static RfVariable_E e_Text("e.Text");
static RfVariable_S s_Delimiter("s.Delimiter");
static RfVariable_S s_Letter("s.Letter");
static RfVariable_S s_Digit("s.Digit");
static RfVariable_S s_Char("s.Char");
static RfVariable_E e_Already("e.Already");
static RfVariable_E e_Bar("e.Bar");
static RfVariable_E e_LineNumbers("e.LineNumbers");
static RfVariable_S s_ErrType("s.ErrType");
static RfVariable_E e_ErrDesc("e.ErrDesc");
static RfVariable_E e_ErrorsList("e.ErrorsList");
static RfVariable_T t_Label("t.Label");
static RfVariable_S s_ErrNum("s.ErrNum");
static RfVariable_T t_N("t.N");
static RfVariable_E e_FileName("e.FileName");
static RfVariable_T t_Error("t.Error");
static RfVariable_T t_LineNumbers("t.LineNumbers");
static RfVariable_E e_Numbers("e.Numbers");
static RfVariable_S s_Number("s.Number");
static RfVariable_T t_ErrorsList("t.ErrorsList");
static RfVariable_E e_NewFileCont("e.NewFileCont");
static RfVariable_T t_Extern("t.Extern");
static RfVariable_S s_LineNumber0("s.LineNumber0");
static RfVariable_S s_ExternType("s.ExternType");
static RfVariable_S s_LineNumber1("s.LineNumber1");
static RfVariable_S s_SymbolType("s.SymbolType");
static RfVariable_E e_Symbol("e.Symbol");
static RfVariable_S s_LineNumber2("s.LineNumber2");
static RfVariable_S s_DelimType("s.DelimType");
static RfVariable_S s_EntryType("s.EntryType");
static RfVariable_S s_LCBracket("s.LCBracket");
static RfVariable_E e_FunCont("e.FunCont");
static RfVariable_S s_LineNumber3("s.LineNumber3");
static RfVariable_S s_RCBracket("s.RCBracket");
static RfVariable_S s_DirectivesType("s.DirectivesType");
static RfVariable_T t_Term("t.Term");
static RfVariable_E e_ExternFuns("e.ExternFuns");
static RfVariable_E e_Ident("e.Ident");
static RfVariable_S s_Type("s.Type");
static RfVariable_S s_LBracket("s.LBracket");
static RfVariable_S s_RBracket("s.RBracket");
static RfVariable_E e_FunName("e.FunName");
static RfVariable_T t_Vars("t.Vars");
static RfVariable_T t_FunName("t.FunName");
static RfVariable_E e_Body("e.Body");
static RfVariable_E e_Clause("e.Clause");
static RfVariable_T t_LineNumber("t.LineNumber");
static RfVariable_E e_Left("e.Left");
static RfVariable_E e_Where("e.Where");
static RfVariable_E e_Right("e.Right");
static RfVariable_S s_LineNumber4("s.LineNumber4");
static RfVariable_S s_LineNumber5("s.LineNumber5");
static RfVariable_E e_RightWhere("e.RightWhere");
static RfVariable_E e_LeftWhere("e.LeftWhere");
static RfVariable_E e_RestWhere("e.RestWhere");
static RfVariable_E e_Var("e.Var");
static RfVariable_E e_Vars("e.Vars");
static RfVariable_S s_VarType("s.VarType");
static RfVariable_E e_Prefix("e.Prefix");
static RfVariable_E e_Suffix("e.Suffix");
static RfVariable_T t_LineNumber1("t.LineNumber1");
static RfVariable_T t_LineNumber2("t.LineNumber2");
static RfVariable_S s_LFBracket("s.LFBracket");
static RfVariable_S s_LineNumberF("s.LineNumberF");
static RfVariable_E e_FunParams("e.FunParams");
static RfVariable_S s_RFBracket("s.RFBracket");
static RfVariable_T t_1("t.1");
static RfVariable_T t_2("t.2");
static RfVariable_T t_3("t.3");
static RfVariable_T t_4("t.4");
static RfVariable_T t_Term1("t.Term1");
static RfVariable_T t_Term2("t.Term2");
static RfVariable_E e_1("e.1");
static RfVariable_S s_Code("s.Code");
static RfVariable_S s_Code1("s.Code1");
static RfVariable_S s_Code2("s.Code2");
static RfVariable_E e_Name("e.Name");
static RfVariable_S s_Name("s.Name");
static RfVariable_E e_SymbName("e.SymbName");
static RfVariable_S s_TextConstantType("s.TextConstantType");
static RfVariable_E e_ModuleName("e.ModuleName");
static RfVariable_E e_FilePath("e.FilePath");
static RfVariable_E e_UsedModule("e.UsedModule");
static RfVariable_S s_DirectType("s.DirectType");
static RfVariable_T t_Foo("t.Foo");
static RfVariable_S s_N("s.N");
static RfVariable_E e_Got("e.Got");
static RfVariable_T t_Got("t.Got");
static RfVariable_E e_FunArgs("e.FunArgs");
static RfVariable_E e_Fun("e.Fun");
static RfVariable_T t_Errors("t.Errors");
static RfVariable_T t_Directives("t.Directives");
static RfVariable_T t_Name("t.Name");
static RfVariable_E e_Entries("e.Entries");
static RfVariable_E e_ExternsDeclared("e.ExternsDeclared");
static RfVariable_E e_Bar1("e.Bar1");
static RfVariable_S s_N1("s.N1");
static RfVariable_E e_Bar2("e.Bar2");
static RfVariable_S s_N2("s.N2");
static RfVariable_E e_Externs("e.Externs");
static RfVariable_E e_Bar3("e.Bar3");
static RfVariable_E e_PublicsDeclared("e.PublicsDeclared");
static RfVariable_E e_NewErrors("e.NewErrors");
static RfVariable_E e_Foo2("e.Foo2");
static RfVariable_E e_Name1("e.Name1");
static RfVariable_E e_Name2("e.Name2");
static RfVariable_E e_CharsConvention("e.CharsConvention");
static RfVariable_E e_NamesConvention("e.NamesConvention");
static RfVariable_E e_Rest("e.Rest");
static RfVariable_E e_HxxFileName("e.HxxFileName");
static RfVariable_E e_PublicFunctions("e.PublicFunctions");
static RfVariable_E e_PublicSymbols("e.PublicSymbols");
static RfVariable_E e_InitFunction("e.InitFunction");
static RfVariable_E e_CxxFileName("e.CxxFileName");
static RfVariable_E e_UsedModules("e.UsedModules");
static RfVariable_E e_ExternalFunctions("e.ExternalFunctions");
static RfVariable_E e_ExternalSymbols("e.ExternalSymbols");
static RfVariable_E e_Bar4("e.Bar4");
static RfVariable_E e_Symbols("e.Symbols");
static RfVariable_E e_Bar5("e.Bar5");
static RfVariable_E e_Bar6("e.Bar6");
static RfVariable_E e_InitFunctionName("e.InitFunctionName");
static RfVariable_E e_LibraryFunction("e.LibraryFunction");
static RfVariable_E e_Library("e.Library");
static RfVariable_E e_Already1("e.Already1");
static RfVariable_E e_Already2("e.Already2");
static RfVariable_E e_Already3("e.Already3");
static RfVariable_S s_FileNumber("s.FileNumber");
static RfVariable_E e_TextConstant("e.TextConstant");
static RfVariable_S s_NumberType("s.NumberType");
static RfVariable_E e_Number("e.Number");
static RfVariable_E e_SemiOrEmpty("e.SemiOrEmpty");


       RfSymbol Start("Start");
static RfSymbol file2_Process("Process");
       RfSymbol Lexical("Lexical");
static RfSymbol file3_FirstLineNumber("FirstLineNumber");
static RfSymbol file3_StateS("StateS");
static RfSymbol file3_StatePragma("StatePragma");
static RfSymbol file3_StatePrefixEmptyPragma("StatePrefixEmptyPragma");
static RfSymbol file3_StateSymbolPragma("StateSymbolPragma");
static RfSymbol file3_StateTextConstantPragma("StateTextConstantPragma");
static RfSymbol file3_StateMultiComment("StateMultiComment");
static RfSymbol file3_StateComment("StateComment");
static RfSymbol file3_StateTextConstant("StateTextConstant");
static RfSymbol file3_StateSymbol("StateSymbol");
static RfSymbol file3_StatePrefixEmpty("StatePrefixEmpty");
static RfSymbol file3_StateInteger("StateInteger");
static RfSymbol file3_StateIntegerEmpty("StateIntegerEmpty");
static RfSymbol file3_StateSuffix("StateSuffix");
static RfSymbol file3_StateFract("StateFract");
static RfSymbol file3_StateNumberSuffixEmpty("StateNumberSuffixEmpty");
static RfSymbol file3_StateIdentSuffixEmpty("StateIdentSuffixEmpty");
static RfSymbol file3_StateFractEmpty("StateFractEmpty");
static RfSymbol file3_IsSpaceOrTab("IsSpaceOrTab");
static RfSymbol file3_IsDelimiter("IsDelimiter");
       RfSymbol IsLetter("IsLetter");
       RfSymbol IsDigit("IsDigit");
       RfSymbol AddError("AddError");
       RfSymbol PrintErrors("PrintErrors");
static RfSymbol file4_NoErrors("NoErrors");
static RfSymbol file4_FirstErrorNumber("FirstErrorNumber");
static RfSymbol file4_CountErrors("CountErrors");
static RfSymbol file4_PrintHeader("PrintHeader");
static RfSymbol file4_CorrectPhrase1("CorrectPhrase1");
static RfSymbol file4_CorrectPhrase2("CorrectPhrase2");
static RfSymbol file4_PrintList("PrintList");
static RfSymbol file4_Description("Description");
static RfSymbol file4_CorrectPhrase3("CorrectPhrase3");
static RfSymbol file4_NumbersList("NumbersList");
static RfSymbol file4_DescriptionBody("DescriptionBody");
static RfSymbol file4_LookAtDesc("LookAtDesc");
       RfSymbol FirstLine("FirstLine");
       RfSymbol LastLine("LastLine");
       RfSymbol GetErrorsList("GetErrorsList");
       RfSymbol CheckLexical("CheckLexical");
       RfSymbol Syntax("Syntax");
static RfSymbol file6_SyntaxMain("SyntaxMain");
static RfSymbol file6_GatherExtern("GatherExtern");
static RfSymbol file6_AddExtern("AddExtern");
static RfSymbol file6_StartSyntax("StartSyntax");
static RfSymbol file6_SyntaxFunCont("SyntaxFunCont");
static RfSymbol file6_StartFun("StartFun");
static RfSymbol file6_SyntaxOneClause("SyntaxOneClause");
static RfSymbol file6_SyntaxWherePart("SyntaxWherePart");
static RfSymbol file6_AddVar("AddVar");
static RfSymbol file6_IsStandard("IsStandard");
static RfSymbol file6_SyntaxLeftPart("SyntaxLeftPart");
static RfSymbol file6_SyntaxRightPart("SyntaxRightPart");
static RfSymbol file6_GetFirstTerm("GetFirstTerm");
static RfSymbol file6_IsSymbolOrConstant("IsSymbolOrConstant");
static RfSymbol file6_CheckCalledFun("CheckCalledFun");
static RfSymbol file6_CheckVar("CheckVar");
       RfSymbol Brackets("Brackets");
static RfSymbol file7_PairBrackets("PairBrackets");
static RfSymbol file7_HasThis("HasThis");
static RfSymbol file7_HasLeftBrackets("HasLeftBrackets");
static RfSymbol file7_FindPair("FindPair");
static RfSymbol file7_IsLeftBracket("IsLeftBracket");
static RfSymbol file7_IsRightBracket("IsRightBracket");
       RfSymbol GetEntryType("GetEntryType");
       RfSymbol GetExternType("GetExternType");
       RfSymbol GetDirectType("GetDirectType");
       RfSymbol GetDelimType("GetDelimType");
       RfSymbol GetTextConstantType("GetTextConstantType");
       RfSymbol GetSymbolType("GetSymbolType");
       RfSymbol GetNumberType("GetNumberType");
       RfSymbol GetVarType("GetVarType");
       RfSymbol GetDelimCode("GetDelimCode");
static RfSymbol file8_GetDecode("GetDecode");
       RfSymbol Reverse2("Reverse2");
       RfSymbol Reverse("Reverse");
       RfSymbol Decode("Decode");
       RfSymbol GetLibraryFuns("GetLibraryFuns");
       RfSymbol Library("Library");
static RfSymbol file9_AddZero("AddZero");
       RfSymbol Unimplemented("Unimplemented");
static RfSymbol file9_NotInStandard("NotInStandard");
static RfSymbol file9_IsUnimplemented("IsUnimplemented");
static RfSymbol file9_CheckBuiltin("CheckBuiltin");
static RfSymbol file9_ProcessBuiltin("ProcessBuiltin");
       RfSymbol CheckSyntax("CheckSyntax");
       RfSymbol SyntaxDirectives("SyntaxDirectives");
static RfSymbol file11_SyntaxOne("SyntaxOne");
       RfSymbol Linker("Linker");
static RfSymbol file12_DirectivesTypesNumber("DirectivesTypesNumber");
static RfSymbol file12_ListsNumber("ListsNumber");
       RfSymbol NextNumbersFrom("NextNumbersFrom");
static RfSymbol file12_GetDirectives("GetDirectives");
static RfSymbol file12_MergeInFiles("MergeInFiles");
static RfSymbol file12_Merge("Merge");
static RfSymbol file12_GetSomething("GetSomething");
static RfSymbol file12_GetSomethingMore("GetSomethingMore");
static RfSymbol file12_GatherRest("GatherRest");
static RfSymbol file12_Gather("Gather");
static RfSymbol file12_GatherMore("GatherMore");
static RfSymbol file12_GatherInFiles("GatherInFiles");
static RfSymbol file12_GatherInOneFile("GatherInOneFile");
static RfSymbol file12_GatherVars("GatherVars");
static RfSymbol file12_GatherSymbols("GatherSymbols");
static RfSymbol file12_GatherEntries("GatherEntries");
static RfSymbol file12_GatherExterns("GatherExterns");
static RfSymbol file12_AddVarOrSymbol("AddVarOrSymbol");
static RfSymbol file12_AddEntry("AddEntry");
       RfSymbol MoreLinker("MoreLinker");
static RfSymbol file13_MakeCheck("MakeCheck");
static RfSymbol file13_ChecksNumber("ChecksNumber");
       RfSymbol NotEmptyPublics("NotEmptyPublics");
static RfSymbol file13_MoreChecks("MoreChecks");
static RfSymbol file13_NoDupesInEntries("NoDupesInEntries");
static RfSymbol file13_NoUnresolvedExternals("NoUnresolvedExternals");
static RfSymbol file13_EveryPublicIsEntry("EveryPublicIsEntry");
       RfSymbol CheckLinker("CheckLinker");
       RfSymbol Generate("Generate");
       RfSymbol GenerateName("GenerateName");
       RfSymbol AddSymbol("AddSymbol");
       RfSymbol TwoNewlines("TwoNewlines");
       RfSymbol IsStatic("IsStatic");
       RfSymbol GenerateHxx("GenerateHxx");
static RfSymbol file16_GetHxxName("GetHxxName");
static RfSymbol file16_HxxPartsNumber("HxxPartsNumber");
static RfSymbol file16_GetHxxCont("GetHxxCont");
static RfSymbol file16_FillHxx("FillHxx");
static RfSymbol file16_StartFilling("StartFilling");
static RfSymbol file16_AddHeader("AddHeader");
static RfSymbol file16_AddInitFunction("AddInitFunction");
       RfSymbol GenerateCxx("GenerateCxx");
static RfSymbol file17_GetCxxName("GetCxxName");
static RfSymbol file17_CxxPartsNumber("CxxPartsNumber");
static RfSymbol file17_GetCxxCont("GetCxxCont");
static RfSymbol file17_FillCxx("FillCxx");
static RfSymbol file17_StartFilling("StartFilling");
static RfSymbol file17_AddHeader("AddHeader");
static RfSymbol file17_AddUsedModules("AddUsedModules");
static RfSymbol file17_AddLibrary("AddLibrary");
static RfSymbol file17_AddAllSymbols("AddAllSymbols");
static RfSymbol file17_AddAllVariables("AddAllVariables");
static RfSymbol file17_GetPrefix("GetPrefix");
static RfSymbol file17_ToUpper("ToUpper");
static RfSymbol file18_FirstFileNumber("FirstFileNumber");
       RfSymbol AddFunctionsCode("AddFunctionsCode");
static RfSymbol file18_StartAdding("StartAdding");
static RfSymbol file18_AddingFromOneFile("AddingFromOneFile");
static RfSymbol file18_NeedsPrefix("NeedsPrefix");
static RfSymbol file18_AddToList("AddToList");
static RfSymbol file18_FinishAdding("FinishAdding");
static RfSymbol file18_AddInitFunction("AddInitFunction");
static RfSymbol file18_MakeInitFunction("MakeInitFunction");
static RfSymbol file18_GenerateFunCont("GenerateFunCont");
static RfSymbol file18_FunctionBody("FunctionBody");
static RfSymbol file18_Clause("Clause");
static RfSymbol file18_Where("Where");
static RfSymbol file18_Expression("Expression");
static RfSymbol file18_SimpleExpression("SimpleExpression");
static RfSymbol file18_Term("Term");
static RfSymbol file18_GenerateInitFunctionArgc("GenerateInitFunctionArgc");
static RfSymbol file18_GenerateRefalFree("GenerateRefalFree");
       RfSymbol GetCurrentVersion("GetCurrentVersion");
       RfSymbol VeryStart("VeryStart");
static RfSymbol file20_ReadFiles("ReadFiles");
static RfSymbol file20_ReadFile("ReadFile");
static RfSymbol file20_ReadFileContents("ReadFileContents");
static RfSymbol file20_DeleteEndZero("DeleteEndZero");
       RfSymbol RemoveSC("RemoveSC");
static RfSymbol file21_IsSemiOrEmpty("IsSemiOrEmpty");


void RefalInit_Start() {
	Start
	||
	(R|e_Files)	^ (R|R<Linker, R<file2_Process, e_Files>R>R);
}

void RefalInit_file2_Process() {
	file2_Process
	||
	(R|t_File, e_Files)	^ (R|R<Syntax, R<Lexical, t_File>R>R, R<file2_Process, e_Files>R)
	||
	(~R)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Process\" function. View field: ", e_Foo>R);
}

void RefalInit_Lexical() {
	Lexical
	||
	(R|(R|t_FileName, e_FileCont))	^ (R|R<CheckLexical, (R|t_FileName, R<file3_StateS, R<file3_FirstLineNumber>R, (R|ErrorsList, "=", (~R)), e_FileCont>R)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"ReadFile\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_FirstLineNumber() {
	file3_FirstLineNumber
	||
	(~R)	^ (R|1);
}

void RefalInit_file3_StateS() {
	file3_StateS
	||
	(R|s_LineNumber, t_Err, s_Symbol, e_Text) & (R|R<file3_IsSpaceOrTab, s_Symbol>R) | (R|True)	^ (R|R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "\n", e_Text)	^ (R|R<file3_StateS, R<Add, s_LineNumber, 1>R, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "/*%%%", e_Text)	^ (R|(R|s_LineNumber, R<GetDirectType>R), (R|s_LineNumber, R<GetDelimType>R, R<GetDelimCode, "{">R), R<file3_StatePragma, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "/*", e_Text)	^ (R|R<file3_StateMultiComment, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "*", e_Text)	^ (R|R<file3_StateComment, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "\'", e_Text)	^ (R|R<file3_StateTextConstant, s_LineNumber, t_Err, (~R), e_Text>R)
	||
	(R|s_LineNumber, t_Err, "\"", e_Text)	^ (R|R<file3_StateSymbol, s_LineNumber, t_Err, (~R), e_Text>R)
	||
	(R|s_LineNumber, t_Err, "$ENTRY", e_Text)	^ (R|(R|s_LineNumber, R<GetEntryType>R), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "$EXTERN", e_Text)	^ (R|(R|s_LineNumber, R<GetExternType>R), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "$EXTRN", e_Text)	^ (R|(R|s_LineNumber, R<GetExternType>R), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "$%%%", e_Text)	^ (R|(R|s_LineNumber, R<GetDirectType>R), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Delimiter, e_Text) & (R|R<file3_IsDelimiter, s_Delimiter>R) | (R|True)	^ (R|(R|s_LineNumber, R<GetDelimType>R, R<GetDelimCode, s_Delimiter>R), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Letter, e_Text) & (R|R<IsLetter, s_Letter>R) | (R|True)	^ (R|R<file3_StatePrefixEmpty, s_LineNumber, t_Err, (R|s_Letter), e_Text>R)
	||
	(R|s_LineNumber, t_Err, "-", e_Text)	^ (R|R<file3_StateInteger, s_LineNumber, t_Err, (R|"-"), e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StateIntegerEmpty, s_LineNumber, t_Err, (R|s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Char, e_Text)	^ (R|R<file3_StateS, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_IllegalChar, s_Char)>R, e_Text>R)
	||
	(R|s_LineNumber, t_Err)	^ (R|t_Err)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateS\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StatePragma() {
	file3_StatePragma
	||
	(R|s_LineNumber, t_Err, "*/", e_Text)	^ (R|(R|s_LineNumber, R<GetDelimType>R, R<GetDelimCode, "}">R), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Symbol, e_Text) & (R|R<file3_IsSpaceOrTab, s_Symbol>R) | (R|True)	^ (R|R<file3_StatePragma, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "\n", e_Text)	^ (R|R<file3_StatePragma, R<Add, s_LineNumber, 1>R, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "\'", e_Text)	^ (R|R<file3_StateTextConstantPragma, s_LineNumber, t_Err, (~R), e_Text>R)
	||
	(R|s_LineNumber, t_Err, "\"", e_Text)	^ (R|R<file3_StateSymbolPragma, s_LineNumber, t_Err, (~R), e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Delimiter, e_Text) & (R|R<file3_IsDelimiter, s_Delimiter>R) | (R|True)	^ (R|(R|s_LineNumber, R<GetDelimType>R, R<GetDelimCode, s_Delimiter>R), R<file3_StatePragma, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Letter, e_Text) & (R|R<IsLetter, s_Letter>R) | (R|True)	^ (R|R<file3_StatePrefixEmptyPragma, s_LineNumber, t_Err, (R|s_Letter), e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Char, e_Text)	^ (R|R<file3_StatePragma, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_IllegalChar, s_Char)>R, e_Text>R)
	||
	(R|s_LineNumber, t_Err)	^ (R|R<file3_StateS, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_BadPragma)>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StatePragma\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StatePrefixEmptyPragma() {
	file3_StatePrefixEmptyPragma
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Letter, e_Text) & (R|R<IsLetter, s_Letter>R) | (R|True)	^ (R|R<file3_StatePrefixEmptyPragma, s_LineNumber, t_Err, (R|e_Already, s_Letter), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "-", e_Text)	^ (R|R<file3_StatePrefixEmptyPragma, s_LineNumber, t_Err, (R|e_Already, "-"), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StatePrefixEmptyPragma, s_LineNumber, t_Err, (R|e_Already, s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), e_Text)	^ (R|(R|s_LineNumber, R<GetSymbolType>R, e_Already), R<file3_StatePragma, s_LineNumber, t_Err, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StatePrefixEmptyPragma\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateSymbolPragma() {
	file3_StateSymbolPragma
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\"", e_Text)	^ (R|(R|s_LineNumber, R<GetSymbolType>R, e_Already), R<file3_StatePragma, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\n", e_Text)	^ (R|(R|s_LineNumber, R<GetSymbolType>R, e_Already), R<file3_StatePragma, R<Add, s_LineNumber, 1>R, R<AddError, t_Err, (R|s_LineNumber, IrinaError_QuoteMissing, "\"")>R, e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\\", s_Char, e_Text)	^ (R|R<file3_StateSymbolPragma, s_LineNumber, t_Err, (R|e_Already, "\\", s_Char), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Char, e_Text)	^ (R|R<file3_StateSymbolPragma, s_LineNumber, t_Err, (R|e_Already, s_Char), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already))	^ (R|R<file3_StatePragma, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_QuoteMissing, "\"")>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateSymbolPragma\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateTextConstantPragma() {
	file3_StateTextConstantPragma
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\'", e_Text)	^ (R|(R|s_LineNumber, R<GetTextConstantType>R, e_Already), R<file3_StatePragma, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\n", e_Text)	^ (R|(R|s_LineNumber, R<GetTextConstantType>R, e_Already), R<file3_StatePragma, R<Add, s_LineNumber, 1>R, R<AddError, t_Err, (R|s_LineNumber, IrinaError_QuoteMissing, "\'")>R, e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\\", s_Char, e_Text)	^ (R|R<file3_StateTextConstantPragma, s_LineNumber, t_Err, (R|e_Already, "\\", s_Char), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\"", e_Text)	^ (R|R<file3_StateTextConstantPragma, s_LineNumber, t_Err, (R|e_Already, "\\\""), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Char, e_Text)	^ (R|R<file3_StateTextConstantPragma, s_LineNumber, t_Err, (R|e_Already, s_Char), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already))	^ (R|R<file3_StatePragma, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_QuoteMissing, "\'")>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateTextConstantPragma\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateMultiComment() {
	file3_StateMultiComment
	||
	(R|s_LineNumber, t_Err, "*/", e_Text)	^ (R|R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, "\n", e_Text)	^ (R|R<file3_StateMultiComment, R<Add, s_LineNumber, 1>R, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Char, e_Text)	^ (R|R<file3_StateMultiComment, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err)	^ (R|R<file3_StateS, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_BadMultiComment)>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateMultiComment\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateComment() {
	file3_StateComment
	||
	(R|s_LineNumber, t_Err, "\n", e_Text)	^ (R|R<file3_StateS, R<Add, s_LineNumber, 1>R, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, s_Char, e_Text)	^ (R|R<file3_StateComment, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err)	^ (R|R<file3_StateS, s_LineNumber, t_Err>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateComment\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateTextConstant() {
	file3_StateTextConstant
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\'", e_Text)	^ (R|(R|s_LineNumber, R<GetTextConstantType>R, e_Already), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\n", e_Text)	^ (R|(R|s_LineNumber, R<GetTextConstantType>R, e_Already), R<file3_StateS, R<Add, s_LineNumber, 1>R, R<AddError, t_Err, (R|s_LineNumber, IrinaError_QuoteMissing, "\'")>R, e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\\", s_Char, e_Text)	^ (R|R<file3_StateTextConstant, s_LineNumber, t_Err, (R|e_Already, "\\", s_Char), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\"", e_Text)	^ (R|R<file3_StateTextConstant, s_LineNumber, t_Err, (R|e_Already, "\\\""), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Char, e_Text)	^ (R|R<file3_StateTextConstant, s_LineNumber, t_Err, (R|e_Already, s_Char), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already))	^ (R|R<file3_StateS, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_QuoteMissing, "\'")>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateTextConstant\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateSymbol() {
	file3_StateSymbol
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\"", e_Text)	^ (R|(R|s_LineNumber, R<GetSymbolType>R, e_Already), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\n", e_Text)	^ (R|(R|s_LineNumber, R<GetSymbolType>R, e_Already), R<file3_StateS, R<Add, s_LineNumber, 1>R, R<AddError, t_Err, (R|s_LineNumber, IrinaError_QuoteMissing, "\"")>R, e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "\\", s_Char, e_Text)	^ (R|R<file3_StateSymbol, s_LineNumber, t_Err, (R|e_Already, "\\", s_Char), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Char, e_Text)	^ (R|R<file3_StateSymbol, s_LineNumber, t_Err, (R|e_Already, s_Char), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already))	^ (R|R<file3_StateS, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_QuoteMissing, "\"")>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateSymbol\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StatePrefixEmpty() {
	file3_StatePrefixEmpty
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Letter, e_Text) & (R|R<IsLetter, s_Letter>R) | (R|True)	^ (R|R<file3_StatePrefixEmpty, s_LineNumber, t_Err, (R|e_Already, s_Letter), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "-", e_Text)	^ (R|R<file3_StatePrefixEmpty, s_LineNumber, t_Err, (R|e_Already, "-"), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StatePrefixEmpty, s_LineNumber, t_Err, (R|e_Already, s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), ".", e_Text)	^ (R|R<file3_StateSuffix, s_LineNumber, t_Err, (R|e_Already, "."), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), e_Text)	^ (R|(R|s_LineNumber, R<GetSymbolType>R, e_Already), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StatePrefixEmpty\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateInteger() {
	file3_StateInteger
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StateIntegerEmpty, s_LineNumber, t_Err, (R|e_Already, s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), e_Text)	^ (R|R<file3_StateS, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_BadMinus)>R, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateInteger\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateIntegerEmpty() {
	file3_StateIntegerEmpty
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StateIntegerEmpty, s_LineNumber, t_Err, (R|e_Already, s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), ".", e_Text)	^ (R|R<file3_StateFract, s_LineNumber, t_Err, (R|e_Already, "."), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), e_Text)	^ (R|(R|s_LineNumber, R<GetNumberType>R, e_Already), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateIntegerEmpty\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateSuffix() {
	file3_StateSuffix
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StateNumberSuffixEmpty, s_LineNumber, t_Err, (R|e_Already, s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Letter, e_Text) & (R|R<IsLetter, s_Letter>R) | (R|True)	^ (R|R<file3_StateIdentSuffixEmpty, s_LineNumber, t_Err, (R|e_Already, s_Letter), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), e_Text)	^ (R|R<file3_StateS, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_BadSuffix, e_Already)>R, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateSuffix\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateFract() {
	file3_StateFract
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StateFractEmpty, s_LineNumber, t_Err, (R|e_Already, s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), e_Text)	^ (R|R<file3_StateS, s_LineNumber, R<AddError, t_Err, (R|s_LineNumber, IrinaError_BadFract, e_Already)>R, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateFract\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateNumberSuffixEmpty() {
	file3_StateNumberSuffixEmpty
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StateNumberSuffixEmpty, s_LineNumber, t_Err, (R|e_Already, s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), e_Text)	^ (R|(R|s_LineNumber, R<GetVarType>R, e_Already), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateNumberSuffixEmpty\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateIdentSuffixEmpty() {
	file3_StateIdentSuffixEmpty
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StateIdentSuffixEmpty, s_LineNumber, t_Err, (R|e_Already, s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Letter, e_Text) & (R|R<IsLetter, s_Letter>R) | (R|True)	^ (R|R<file3_StateIdentSuffixEmpty, s_LineNumber, t_Err, (R|e_Already, s_Letter), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), "-", e_Text)	^ (R|R<file3_StateIdentSuffixEmpty, s_LineNumber, t_Err, (R|e_Already, "-"), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), e_Text)	^ (R|(R|s_LineNumber, R<GetVarType>R, e_Already), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateIdentSuffixEmpty\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_StateFractEmpty() {
	file3_StateFractEmpty
	||
	(R|s_LineNumber, t_Err, (R|e_Already), s_Digit, e_Text) & (R|R<IsDigit, s_Digit>R) | (R|True)	^ (R|R<file3_StateFractEmpty, s_LineNumber, t_Err, (R|e_Already, s_Digit), e_Text>R)
	||
	(R|s_LineNumber, t_Err, (R|e_Already), e_Text)	^ (R|(R|s_LineNumber, R<GetNumberType>R, e_Already), R<file3_StateS, s_LineNumber, t_Err, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StateFractEmpty\" function. View field: ", e_Foo>R);
}

void RefalInit_file3_IsSpaceOrTab() {
	file3_IsSpaceOrTab
	||
	(R|" ")	^ (R|True)
	||
	(R|"\t")	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_file3_IsDelimiter() {
	file3_IsDelimiter
	||
	(R|s_Delimiter) & (R|"{}()<>=;,:") | (R|e_Foo, s_Delimiter, e_Bar)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_IsLetter() {
	IsLetter
	||
	(R|s_Letter) & (R|"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_") | (R|e_Foo, s_Letter, e_Bar)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_IsDigit() {
	IsDigit
	||
	(R|s_Digit) & (R|"0123456789") | (R|e_Foo, s_Digit, e_Bar)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_AddError() {
	AddError
	||
	(R|(R|ErrorsList, "=", (R|e_Foo, (R|(R|e_LineNumbers, s_LineNumber), s_ErrType, e_ErrDesc), e_Bar)), (R|s_LineNumber, s_ErrType, e_ErrDesc))	^ (R|(R|ErrorsList, "=", (R|e_Foo, (R|(R|e_LineNumbers, s_LineNumber), s_ErrType, e_ErrDesc), e_Bar)))
	||
	(R|(R|ErrorsList, "=", (R|e_Foo, (R|(R|e_LineNumbers), s_ErrType, e_ErrDesc), e_Bar)), (R|s_LineNumber, s_ErrType, e_ErrDesc))	^ (R|(R|ErrorsList, "=", (R|e_Foo, (R|(R|e_LineNumbers, s_LineNumber), s_ErrType, e_ErrDesc), e_Bar)))
	||
	(R|(R|ErrorsList, "=", (R|e_ErrorsList)), (R|s_LineNumber, s_ErrType, e_ErrDesc))	^ (R|(R|ErrorsList, "=", (R|e_ErrorsList, (R|(R|s_LineNumber), s_ErrType, e_ErrDesc))))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddError\" function. View field: ", e_Foo>R);
}

void RefalInit_PrintErrors() {
	PrintErrors
	||
	(R|t_Label, e_ErrorsList)	^ (R|R<file4_PrintHeader, t_Label, R<file4_CountErrors, R<file4_NoErrors>R, e_ErrorsList>R>R, R<file4_PrintList, R<file4_FirstErrorNumber>R, e_ErrorsList>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"PrintErrors\" function. View field: ", e_Foo>R);
}

void RefalInit_file4_NoErrors() {
	file4_NoErrors
	||
	(~R)	^ (R|0);
}

void RefalInit_file4_FirstErrorNumber() {
	file4_FirstErrorNumber
	||
	(~R)	^ (R|1);
}

void RefalInit_file4_CountErrors() {
	file4_CountErrors
	||
	(R|s_ErrNum, t_Err, e_ErrorsList)	^ (R|R<file4_CountErrors, R<Add, s_ErrNum, 1>R, e_ErrorsList>R)
	||
	(R|s_ErrNum)	^ (R|s_ErrNum)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"CountErrors\" function. View field: ", e_Foo>R);
}

void RefalInit_file4_PrintHeader() {
	file4_PrintHeader
	||
	(R|t_Label, s_ErrNum)	^ (R|R<Prout, R<Symb, s_ErrNum>R, " ", R<file4_CorrectPhrase1, s_ErrNum>R, "found ", R<file4_CorrectPhrase2, t_Label>R, ":">R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"PrintHeader\" function. View field: ", e_Foo>R);
}

void RefalInit_file4_CorrectPhrase1() {
	file4_CorrectPhrase1
	||
	(R|1)	^ (R|"error was ")
	||
	(R|t_N)	^ (R|"errors were ")
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"CorrectPhrase1\" function. View field: ", e_Foo>R);
}

void RefalInit_file4_CorrectPhrase2() {
	file4_CorrectPhrase2
	||
	(R|(R|FileName, "=", (R|e_FileName)))	^ (R|"in file \"", e_FileName, "\"")
	||
	(R|LinkerErrors)	^ (R|"during link edition")
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"CorrectPhrase2\" function. View field: ", e_Foo>R);
}

void RefalInit_file4_PrintList() {
	file4_PrintList
	||
	(R|s_ErrNum, t_Error, e_ErrorsList)	^ (R|R<Prout, R<Symb, s_ErrNum>R, ". ", R<file4_Description, t_Error>R>R, R<file4_PrintList, R<Add, s_ErrNum, 1>R, e_ErrorsList>R)
	||
	(R|s_ErrNum)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"PrintList\" function. View field: ", e_Foo>R);
}

void RefalInit_file4_Description() {
	file4_Description
	||
	(R|(R|t_LineNumbers, s_ErrType, e_ErrDesc))	^ (R|R<file4_DescriptionBody, s_ErrType>R, R<file4_LookAtDesc, e_ErrDesc>R, R<file4_CorrectPhrase3, t_LineNumbers>R, ".")
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Description\" function. View field: ", e_Foo>R);
}

void RefalInit_file4_CorrectPhrase3() {
	file4_CorrectPhrase3
	||
	(R|(R|0))	^ (~R)
	||
	(R|(R|s_LineNumber))	^ (R|" on line ", R<Symb, s_LineNumber>R)
	||
	(R|(R|s_LineNumber, e_Numbers))	^ (R|" on lines ", R<Symb, s_LineNumber>R, R<file4_NumbersList, e_Numbers>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"CorrectPhrase3\" function. View field: ", e_Foo>R);
}

void RefalInit_file4_NumbersList() {
	file4_NumbersList
	||
	(R|s_Number, e_Numbers)	^ (R|", ", R<Symb, s_Number>R, R<file4_NumbersList, e_Numbers>R)
	||
	(~R)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"NumbersList\" function. View field: ", e_Foo>R);
}

void RefalInit_file4_DescriptionBody() {
	file4_DescriptionBody
	||
	(R|IrinaError_NoPairFor)	^ (R|"No pair for")
	||
	(R|IrinaError_IllegalExpr)	^ (R|"Illegal expression")
	||
	(R|IrinaError_IllegalChar)	^ (R|"Illegal character")
	||
	(R|IrinaError_BadMultiComment)	^ (R|"Comment reaches end of file")
	||
	(R|IrinaError_QuoteMissing)	^ (R|"Missing quote")
	||
	(R|IrinaError_BadMinus)	^ (R|"No number after \"-\"")
	||
	(R|IrinaError_BadSuffix)	^ (R|"No suffix after")
	||
	(R|IrinaError_BadFract)	^ (R|"No fraction after")
	||
	(R|IrinaError_EntryRedefinition)	^ (R|"Redefinition of entry function")
	||
	(R|IrinaError_UnresolvedExternal)	^ (R|"Unresolved external")
	||
	(R|IrinaError_Redefinition)	^ (R|"Redefinition of")
	||
	(R|IrinaError_EmptyFun)	^ (R|"Empty function")
	||
	(R|IrinaError_MatchMissing)	^ (R|"Missing \"=\"")
	||
	(R|IrinaError_ColonMissing)	^ (R|"Missing \":\"")
	||
	(R|IrinaError_BadFuncall)	^ (R|"No function name in function call")
	||
	(R|IrinaError_UndefinedFun)	^ (R|"Undefined function")
	||
	(R|IrinaError_UndefinedVar)	^ (R|"Undefined variable")
	||
	(R|IrinaError_NonStandardPrefix)	^ (R|"Unsupported in version ", R<GetCurrentVersion>R, " non-standard type")
	||
	(R|IrinaError_UnimplementedLibFun)	^ (R|"Unimplemented in version ", R<GetCurrentVersion>R, " library function")
	||
	(R|IrinaError_PublicIsNotEntry)	^ (R|"Missing or not preceded with \"$ENTRY\" public function")
	||
	(R|e_Foo)	^ (R|e_Foo);
}

void RefalInit_file4_LookAtDesc() {
	file4_LookAtDesc
	||
	(~R)	^ (~R)
	||
	(R|"\"")	^ (R|" \'\"\'")
	||
	(R|e_Foo)	^ (R|" \"", R<Decode, e_Foo>R, "\"");
}

void RefalInit_FirstLine() {
	FirstLine
	||
	(R|(R|s_LineNumber, e_Bar), e_Foo)	^ (R|s_LineNumber)
	||
	(R|(R|e_Foo), e_Text)	^ (R|R<FirstLine, e_Foo>R)
	||
	(~R)	^ (R|0)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"FirstLine\" function. View field: ", e_Foo>R);
}

void RefalInit_LastLine() {
	LastLine
	||
	(R|e_Foo, (R|s_LineNumber, e_Bar))	^ (R|s_LineNumber)
	||
	(R|e_Text, (R|e_Foo))	^ (R|R<LastLine, e_Foo>R)
	||
	(~R)	^ (R|0)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"LastLine\" function. View field: ", e_Foo>R);
}

void RefalInit_GetErrorsList() {
	GetErrorsList
	||
	(R|(R|ErrorsList, "=", (R|e_Already)))	^ (R|(R|e_Already))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GetErrorsList\" function. View field: ", e_Foo>R);
}

void RefalInit_CheckLexical() {
	CheckLexical
	||
	(R|(R|t_FileName, e_FileCont, (R|ErrorsList, "=", (~R))))	^ (R|(R|t_FileName, (R|ErrorsList, "=", (~R)), e_FileCont))
	||
	(R|(R|t_FileName, e_FileCont, (R|ErrorsList, "=", (R|e_ErrorsList))))	^ (R|R<PrintErrors, (R|FileName, "=", t_FileName), e_ErrorsList>R, (~R))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"CheckLexical\" function. View field: ", e_Foo>R);
}

void RefalInit_Syntax() {
	Syntax
	||
	(R|(R|t_FileName, t_ErrorsList, e_FileCont))	^ (R|R<file6_SyntaxMain, (R|t_FileName, R<Brackets, t_ErrorsList, e_FileCont>R)>R)
	||
	(R|(~R))	^ (R|(~R))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Syntax\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_SyntaxMain() {
	file6_SyntaxMain
	||
	(R|(R|t_FileName, t_ErrorsList, e_FileCont)) & (R|R<RemoveSC, e_FileCont>R) | (R|e_NewFileCont)	^ (R|R<CheckSyntax, (R|t_FileName, R<file6_StartSyntax, R<Reverse2, R<file6_GatherExtern, t_ErrorsList, (R|ExternFuns, "=", (R|R<GetLibraryFuns>R)), e_NewFileCont>R>R>R, e_NewFileCont)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"SyntaxMain\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_GatherExtern() {
	file6_GatherExtern
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber0, s_ExternType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|s_LineNumber2, s_DelimType, Semicolon), e_Text) & (R|R<GetExternType>R) | (R|s_ExternType) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|(R|s_LineNumber0, s_ExternType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|s_LineNumber2, s_DelimType, Semicolon), R<file6_GatherExtern, R<file6_AddExtern, t_ErrorsList, t_Extern, (R|s_LineNumber2, s_SymbolType, e_Symbol)>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber0, s_EntryType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetEntryType>R) | (R|s_EntryType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|(R|s_LineNumber0, s_EntryType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), R<file6_GatherExtern, R<file6_AddExtern, t_ErrorsList, t_Extern, (R|s_LineNumber1, s_SymbolType, e_Symbol)>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber0, s_DirectivesType), (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetDirectType>R) | (R|s_DirectivesType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|(R|s_LineNumber0, s_DirectivesType), (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), R<file6_GatherExtern, t_ErrorsList, t_Extern, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|(R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), R<file6_GatherExtern, R<file6_AddExtern, t_ErrorsList, t_Extern, (R|s_LineNumber1, s_SymbolType, e_Symbol)>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_Term, e_Text)	^ (R|t_Term, R<file6_GatherExtern, t_ErrorsList, t_Extern, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern)	^ (R|t_ErrorsList, t_Extern)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GatherExtern\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_AddExtern() {
	file6_AddExtern
	||
	(R|t_ErrorsList, (R|ExternFuns, "=", (R|e_Foo, (R|s_LineNumber1, e_Symbol), e_Bar)), (R|s_LineNumber2, s_SymbolType, e_Symbol))	^ (R|R<AddError, t_ErrorsList, (R|s_LineNumber2, IrinaError_Redefinition, e_Symbol)>R, (R|ExternFuns, "=", (R|e_Foo, (R|s_LineNumber1, e_Symbol), e_Bar)))
	||
	(R|t_ErrorsList, (R|ExternFuns, "=", (R|e_ExternFuns)), (R|s_LineNumber, s_SymbolType, e_Symbol))	^ (R|t_ErrorsList, (R|ExternFuns, "=", (R|e_ExternFuns, (R|s_LineNumber, e_Symbol))))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddExtern\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_StartSyntax() {
	file6_StartSyntax
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber0, s_ExternType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|s_LineNumber2, s_DelimType, Semicolon), e_Text) & (R|R<GetExternType>R) | (R|s_ExternType) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|R<file6_StartSyntax, t_ErrorsList, t_Extern, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber0, s_EntryType), (R|s_LineNumber1, s_SymbolType, e_Ident), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetEntryType>R) | (R|s_EntryType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file6_StartSyntax, R<file6_SyntaxFunCont, t_ErrorsList, t_Extern, (R|s_LineNumber1, e_Ident), (~R), e_FunCont>R, t_Extern, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber0, s_DirectivesType), (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetDirectType>R) | (R|s_DirectivesType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file6_StartSyntax, R<SyntaxDirectives, t_ErrorsList, e_Foo>R, t_Extern, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber1, s_SymbolType, e_Ident), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file6_StartSyntax, R<file6_SyntaxFunCont, t_ErrorsList, t_Extern, (R|s_LineNumber1, e_Ident), (~R), e_FunCont>R, t_Extern, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber, s_Type, e_Foo), e_Text)	^ (R|R<file6_StartSyntax, R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_IllegalExpr, e_Foo)>R, t_Extern, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, (R|(R|s_LineNumber1, s_DelimType, s_LBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RBracket)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType)	^ (R|R<file6_StartSyntax, R<file6_StartSyntax, R<AddError, t_ErrorsList, (R|s_LineNumber1, IrinaError_IllegalExpr, s_LBracket)>R, t_Extern, e_Foo>R, t_Extern, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StartSyntax\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_SyntaxFunCont() {
	file6_SyntaxFunCont
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber, e_FunName), t_Vars)	^ (R|R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_EmptyFun, e_FunName)>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Body)	^ (R|R<file6_StartFun, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Body>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"SyntaxFunCont\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_StartFun() {
	file6_StartFun
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Clause, (R|t_LineNumber, s_DelimType, Semicolon), e_Text)	^ (R|R<file6_StartFun, R<file6_SyntaxOneClause, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Clause, (R|t_LineNumber, R<GetDelimType>R, Semicolon)>R, t_Extern, t_FunName, t_Vars, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Text)	^ (R|R<file6_SyntaxOneClause, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Text, (R|R<LastLine, e_Text>R, R<GetDelimType>R, Semicolon)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StartFun\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_SyntaxOneClause() {
	file6_SyntaxOneClause
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Left, (R|s_LineNumber1, s_DelimType, Comma), e_Where, (R|s_LineNumber2, s_DelimType, Equal), e_Right, (R|s_LineNumber3, s_DelimType, Semicolon))	^ (R|R<file6_GetFirstTerm, R<file6_SyntaxRightPart, R<file6_SyntaxWherePart, R<file6_SyntaxLeftPart, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Left>R, (R|s_LineNumber1, s_DelimType, Comma), e_Where>R, e_Right>R>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Left, (R|s_LineNumber1, s_DelimType, Equal), e_Right, (R|s_LineNumber2, s_DelimType, Semicolon))	^ (R|R<file6_GetFirstTerm, R<file6_SyntaxRightPart, R<file6_SyntaxLeftPart, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Left>R, e_Right>R>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Left, (R|s_LineNumber1, s_DelimType, Comma), e_Where, (R|s_LineNumber2, s_DelimType, Colon), (R|(R|s_LineNumber3, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber4, s_DelimType, s_RCBracket)), (R|s_LineNumber5, s_DelimType, Semicolon)) & (R|R<GetDelimType>R) | (R|s_DelimType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file6_SyntaxFunCont, R<file6_SyntaxWherePart, R<file6_SyntaxLeftPart, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Left>R, (R|s_LineNumber1, R<GetDelimType>R, Comma), e_Where, (R|s_LineNumber2, R<GetDelimType>R, Colon)>R, e_FunCont>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Text, (R|s_LineNumber, s_DelimType, Semicolon))	^ (R|R<file6_SyntaxOneClause, R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_MatchMissing)>R, t_Extern, t_FunName, t_Vars, e_Text, (R|s_LineNumber, R<GetDelimType>R, Equal), (R|s_LineNumber, R<GetDelimType>R, Semicolon)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"SyntaxOneClause\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_SyntaxWherePart() {
	file6_SyntaxWherePart
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber1, s_DelimType, Comma), e_RightWhere, (R|s_LineNumber2, s_DelimType, Colon), e_LeftWhere, (R|s_LineNumber3, s_DelimType, Comma), e_RestWhere)	^ (R|R<file6_SyntaxWherePart, R<file6_SyntaxLeftPart, R<file6_SyntaxRightPart, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_RightWhere>R, e_LeftWhere>R, (R|s_LineNumber3, R<GetDelimType>R, Comma), e_RestWhere>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber1, s_DelimType, Comma), e_RightWhere, (R|s_LineNumber2, s_DelimType, Colon), e_LeftWhere)	^ (R|R<file6_SyntaxLeftPart, R<file6_SyntaxRightPart, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_RightWhere>R, e_LeftWhere>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber1, s_DelimType, Comma), e_Text)	^ (R|R<file6_SyntaxWherePart, R<AddError, t_ErrorsList, (R|s_LineNumber1, IrinaError_ColonMissing)>R, t_Extern, t_FunName, t_Vars, (R|s_LineNumber1, R<GetDelimType>R, Comma), (R|s_LineNumber1, R<GetDelimType>R, Colon), e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"SyntaxWherePart\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_AddVar() {
	file6_AddVar
	||
	(R|(R|e_Foo, (R|e_Var), e_Bar), (R|e_Var))	^ (R|(R|e_Foo, (R|e_Var), e_Bar))
	||
	(R|(R|e_Vars), (R|e_Var))	^ (R|(R|e_Vars, (R|e_Var)))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddVar\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_IsStandard() {
	file6_IsStandard
	||
	(R|e_Text) & (R|(R|"s"), (R|"e"), (R|"t")) | (R|e_Foo, e_Text, e_Bar)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_file6_SyntaxLeftPart() {
	file6_SyntaxLeftPart
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber, s_VarType, e_Prefix, ".", e_Suffix), e_Text) & (R|R<GetVarType>R) | (R|s_VarType) & (R|R<file6_IsStandard, (R|e_Prefix)>R) | (R|False)	^ (R|R<file6_SyntaxLeftPart, R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_NonStandardPrefix, e_Prefix)>R, t_Extern, t_FunName, R<file6_AddVar, t_Vars, (R|e_Prefix, ".", e_Suffix)>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber1, s_VarType, e_Var), e_Text) & (R|R<GetVarType>R) | (R|s_VarType)	^ (R|R<file6_SyntaxLeftPart, t_ErrorsList, t_Extern, t_FunName, R<file6_AddVar, t_Vars, (R|e_Var)>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber1, s_Type, e_Ident), e_Text) & (R|R<file6_IsSymbolOrConstant, s_Type>R) | (R|True)	^ (R|R<file6_SyntaxLeftPart, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|(R|s_LineNumber1, s_DelimType, s_LBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RBracket)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType) & (R|R<GetDelimCode, "(">R) | (R|s_LBracket) & (R|R<GetDelimCode, ")">R) | (R|s_RBracket)	^ (R|R<file6_SyntaxLeftPart, R<file6_SyntaxLeftPart, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Foo>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber, s_Type, e_Foo), e_Text)	^ (R|R<file6_SyntaxLeftPart, R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_IllegalExpr, e_Foo)>R, t_Extern, t_FunName, t_Vars, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|(R|t_LineNumber1, s_DelimType, s_LBracket), e_Foo, (R|t_LineNumber2, s_DelimType, s_RBracket)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType)	^ (R|R<file6_SyntaxLeftPart, R<file6_SyntaxLeftPart, R<AddError, t_ErrorsList, (R|t_LineNumber1, IrinaError_IllegalExpr, s_LBracket)>R, t_Extern, t_FunName, t_Vars, e_Foo>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars)	^ (R|t_ErrorsList, t_Extern, t_FunName, t_Vars)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"SyntaxLeftPart\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_SyntaxRightPart() {
	file6_SyntaxRightPart
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber1, s_VarType, e_Var), e_Text) & (R|R<GetVarType>R) | (R|s_VarType)	^ (R|R<file6_SyntaxRightPart, R<file6_CheckVar, t_ErrorsList, t_Vars, (R|s_LineNumber1, e_Var)>R, t_Extern, t_FunName, t_Vars, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber1, s_Type, e_Ident), e_Text) & (R|R<file6_IsSymbolOrConstant, s_Type>R) | (R|True)	^ (R|R<file6_SyntaxRightPart, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|(R|s_LineNumber1, s_DelimType, s_LBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RBracket)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType) & (R|R<GetDelimCode, "(">R) | (R|s_LBracket) & (R|R<GetDelimCode, ")">R) | (R|s_RBracket)	^ (R|R<file6_SyntaxRightPart, R<file6_SyntaxRightPart, t_ErrorsList, t_Extern, t_FunName, t_Vars, e_Foo>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|(R|s_LineNumber1, s_DelimType, s_LFBracket), (R|s_LineNumberF, s_SymbolType, e_FunName), e_FunParams, (R|s_LineNumber2, s_DelimType, s_RFBracket)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "<">R) | (R|s_LFBracket) & (R|R<GetDelimCode, ">">R) | (R|s_RFBracket)	^ (R|R<file6_SyntaxRightPart, R<file6_SyntaxRightPart, R<file6_CheckCalledFun, t_ErrorsList, t_Extern, (R|s_LineNumberF, e_FunName)>R, t_Extern, t_FunName, t_Vars, e_FunParams>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|s_LineNumber, s_Type, e_Foo), e_Text)	^ (R|R<file6_SyntaxRightPart, R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_IllegalExpr, e_Foo)>R, t_Extern, t_FunName, t_Vars, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|(R|s_LineNumber1, s_DelimType, s_LFBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RFBracket)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType) & (R|R<GetDelimCode, "<">R) | (R|s_LFBracket) & (R|R<GetDelimCode, ">">R) | (R|s_RFBracket)	^ (R|R<file6_SyntaxRightPart, R<file6_SyntaxRightPart, R<AddError, t_ErrorsList, (R|s_LineNumber1, IrinaError_BadFuncall)>R, t_Extern, t_FunName, t_Vars, e_Foo>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars, (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file6_SyntaxRightPart, R<file6_SyntaxRightPart, R<AddError, t_ErrorsList, (R|s_LineNumber1, IrinaError_IllegalExpr, s_LCBracket)>R, t_Extern, t_FunName, t_Vars, e_Foo>R, e_Text>R)
	||
	(R|t_ErrorsList, t_Extern, t_FunName, t_Vars)	^ (R|t_ErrorsList, t_Extern, t_FunName, t_Vars)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"SyntaxRightPart\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_GetFirstTerm() {
	file6_GetFirstTerm
	||
	(R|t_1, t_2, t_3, t_4)	^ (R|t_1)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GetFirstTerm\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_IsSymbolOrConstant() {
	file6_IsSymbolOrConstant
	||
	(R|s_Type) & (R|R<GetSymbolType>R, R<GetTextConstantType>R, R<GetNumberType>R) | (R|e_Foo, s_Type, e_Bar)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_file6_CheckCalledFun() {
	file6_CheckCalledFun
	||
	(R|t_ErrorsList, (R|ExternFuns, "=", (R|e_Foo, (R|s_LineNumber1, e_FunName), e_Bar)), (R|s_LineNumber2, e_FunName))	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber, e_FunName)) & (R|R<Unimplemented>R) | (R|e_Foo, (R|e_FunName), e_Bar)	^ (R|R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_UnimplementedLibFun, e_FunName)>R)
	||
	(R|t_ErrorsList, t_Extern, (R|s_LineNumber, e_FunName))	^ (R|R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_UndefinedFun, e_FunName)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"CheckCalledFun\" function. View field: ", e_Foo>R);
}

void RefalInit_file6_CheckVar() {
	file6_CheckVar
	||
	(R|t_ErrorsList, (R|e_Foo, (R|e_Var), e_Bar), (R|s_LineNumber, e_Var))	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, t_Vars, (R|s_LineNumber, e_Var))	^ (R|R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_UndefinedVar, e_Var)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"CheckVar\" function. View field: ", e_Foo>R);
}

void RefalInit_Brackets() {
	Brackets
	||
	(R|t_ErrorsList, e_Foo, (R|s_LineNumber, s_DelimType, s_RBracket), e_Bar) & (R|R<file7_IsRightBracket, s_RBracket>R) | (R|True)	^ (R|R<Brackets, R<file7_PairBrackets, t_ErrorsList, R<file7_FindPair, s_RBracket>R, e_Foo, (R|s_LineNumber, s_DelimType, s_RBracket)>R, e_Bar>R)
	||
	(R|t_ErrorsList, e_Foo, (R|s_LineNumber, s_DelimType, s_LBracket), e_Bar) & (R|R<file7_IsLeftBracket, s_LBracket>R) | (R|True) & (R|R<file7_HasLeftBrackets, e_Bar>R) | (R|False)	^ (R|R<Brackets, R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_NoPairFor, s_LBracket)>R, e_Foo, (R|s_LineNumber, s_DelimType, s_LBracket), e_Bar, (R|R<LastLine, e_Bar>R, s_DelimType, R<file7_FindPair, s_LBracket>R)>R)
	||
	(R|t_ErrorsList, e_Foo)	^ (R|t_ErrorsList, e_Foo)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Brackets\" function. View field: ", e_Foo>R);
}

void RefalInit_file7_PairBrackets() {
	file7_PairBrackets
	||
	(R|t_ErrorsList, s_LBracket, e_Foo, (R|s_LineNumber, s_DelimType, s_LBracket), e_Bar) & (R|R<file7_HasThis, s_LBracket, e_Bar>R) | (R|False)	^ (R|t_ErrorsList, e_Foo, (R|(R|s_LineNumber, s_DelimType, s_LBracket), e_Bar))
	||
	(R|t_ErrorsList, s_LBracket, e_Foo, (R|s_LineNumber, s_DelimType, s_RBracket))	^ (R|R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_NoPairFor, s_RBracket)>R, (R|(R|R<FirstLine, e_Foo>R, s_DelimType, s_LBracket), e_Foo, (R|s_LineNumber, s_DelimType, s_RBracket)))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"PairBrackets\" function. View field: ", e_Foo>R);
}

void RefalInit_file7_HasThis() {
	file7_HasThis
	||
	(R|s_Symbol, e_Foo, (R|t_LineNumber, s_Type, s_Symbol), e_Bar)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_file7_HasLeftBrackets() {
	file7_HasLeftBrackets
	||
	(R|e_Foo, s_LBracket, e_Bar) & (R|R<file7_IsLeftBracket, s_LBracket>R) | (R|True)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_file7_FindPair() {
	file7_FindPair
	||
	(R|s_Symbol) & (R|R<GetDelimCode, "{">R) | (R|s_Symbol)	^ (R|R<GetDelimCode, "}">R)
	||
	(R|s_Symbol) & (R|R<GetDelimCode, "}">R) | (R|s_Symbol)	^ (R|R<GetDelimCode, "{">R)
	||
	(R|s_Symbol) & (R|R<GetDelimCode, "(">R) | (R|s_Symbol)	^ (R|R<GetDelimCode, ")">R)
	||
	(R|s_Symbol) & (R|R<GetDelimCode, ")">R) | (R|s_Symbol)	^ (R|R<GetDelimCode, "(">R)
	||
	(R|s_Symbol) & (R|R<GetDelimCode, "<">R) | (R|s_Symbol)	^ (R|R<GetDelimCode, ">">R)
	||
	(R|s_Symbol) & (R|R<GetDelimCode, ">">R) | (R|s_Symbol)	^ (R|R<GetDelimCode, "<">R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"FindPair\" function. View field: ", e_Foo>R);
}

void RefalInit_file7_IsLeftBracket() {
	file7_IsLeftBracket
	||
	(R|s_LBracket) & (R|R<GetDelimCode, "{">R, R<GetDelimCode, "(">R, R<GetDelimCode, "<">R) | (R|e_Foo, s_LBracket, e_Bar)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_file7_IsRightBracket() {
	file7_IsRightBracket
	||
	(R|s_RBracket) & (R|R<GetDelimCode, "}">R, R<GetDelimCode, ")">R, R<GetDelimCode, ">">R) | (R|e_Foo, s_RBracket, e_Bar)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_GetEntryType() {
	GetEntryType
	||
	(~R)	^ (R|TypeEntry);
}

void RefalInit_GetExternType() {
	GetExternType
	||
	(~R)	^ (R|TypeExtern);
}

void RefalInit_GetDirectType() {
	GetDirectType
	||
	(~R)	^ (R|TypeDirect);
}

void RefalInit_GetDelimType() {
	GetDelimType
	||
	(~R)	^ (R|TypeDelim);
}

void RefalInit_GetTextConstantType() {
	GetTextConstantType
	||
	(~R)	^ (R|TypeText);
}

void RefalInit_GetSymbolType() {
	GetSymbolType
	||
	(~R)	^ (R|TypeSymbol);
}

void RefalInit_GetNumberType() {
	GetNumberType
	||
	(~R)	^ (R|TypeNumber);
}

void RefalInit_GetVarType() {
	GetVarType
	||
	(~R)	^ (R|TypeVar);
}

void RefalInit_GetDelimCode() {
	GetDelimCode
	||
	(R|"{")	^ (R|LCurBracket)
	||
	(R|"}")	^ (R|RCurBracket)
	||
	(R|"(")	^ (R|LBracket)
	||
	(R|")")	^ (R|RBracket)
	||
	(R|"<")	^ (R|LFunBracket)
	||
	(R|">")	^ (R|RFunBracket)
	||
	(R|"=")	^ (R|Equal)
	||
	(R|";")	^ (R|Semicolon)
	||
	(R|",")	^ (R|Comma)
	||
	(R|":")	^ (R|Colon)
	||
	(R|e_Foo)	^ (R|e_Foo);
}

void RefalInit_file8_GetDecode() {
	file8_GetDecode
	||
	(R|LCurBracket)	^ (R|"{")
	||
	(R|RCurBracket)	^ (R|"}")
	||
	(R|LBracket)	^ (R|"(")
	||
	(R|RBracket)	^ (R|")")
	||
	(R|LFunBracket)	^ (R|"<")
	||
	(R|RFunBracket)	^ (R|">")
	||
	(R|Equal)	^ (R|"=")
	||
	(R|Semicolon)	^ (R|";")
	||
	(R|Comma)	^ (R|",")
	||
	(R|Colon)	^ (R|":")
	||
	(R|e_Foo)	^ (R|e_Foo);
}

void RefalInit_Reverse2() {
	Reverse2
	||
	(R|e_Foo, t_Term1, t_Term2)	^ (R|t_Term1, t_Term2, e_Foo)
	||
	(R|e_Foo)	^ (R|e_Foo);
}

void RefalInit_Reverse() {
	Reverse
	||
	(R|e_1, t_2)	^ (R|t_2, e_1)
	||
	(R|e_Foo)	^ (R|e_Foo);
}

void RefalInit_Decode() {
	Decode
	||
	(R|s_Code, e_Text)	^ (R|R<file8_GetDecode, s_Code>R, R<Decode, e_Text>R)
	||
	(R|(R|(R|s_LineNumber1, s_DelimType, s_Code1), e_Foo, (R|s_LineNumber2, s_DelimType, s_Code2)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType)	^ (R|R<file8_GetDecode, s_Code1>R, R<Decode, e_Foo>R, R<file8_GetDecode, s_Code2>R, R<Decode, e_Text>R)
	||
	(R|e_Foo)	^ (R|e_Foo);
}

void RefalInit_GetLibraryFuns() {
	GetLibraryFuns
	||
	(~R)	^ (R|R<file9_AddZero, R<Library>R>R);
}

void RefalInit_Library() {
	Library
	||
	(~R)	^ (R|R<file9_CheckBuiltin, R<ListOfBuiltin>R>R);
}

void RefalInit_file9_AddZero() {
	file9_AddZero
	||
	(R|(R|e_Foo), e_Text)	^ (R|(R|0, e_Foo), R<file9_AddZero, e_Text>R)
	||
	(~R)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddZero\" function. View field: ", e_Foo>R);
}

void RefalInit_Unimplemented() {
	Unimplemented
	||
	(~R)	^ (R|(R|"Divmod"), (R|"First"), (R|"Last"), (R|"Implode"), (R|"System"), (R|"Step"), (R|"Random"), (R|"RandomDigit"), (R|"Write"), (R|"Lower"), (R|"Upper"), (R|"Dn"), (R|"Up"), (R|"XMLParse"), (R|"TimeElapsed"), (R|"Sysfun"), (R|"Imp$$"), (R|"Stop$$"), (R|"Freeze"), (R|"Freezer"), (R|"Ev-met"), (R|"Residue"), (R|"Implode_Ext"), (R|"Explode_Ext"), (R|"TimeElapsed"), (R|"DeSysfun"), (R|"SizeOf"), (R|"int4fab_1"), (R|""));
}

void RefalInit_file9_NotInStandard() {
	file9_NotInStandard
	||
	(~R)	^ (R|(R|"Argc"), (R|"Real"), (R|"Trunc"), (R|"Sin"), (R|"Cos"), (R|"Tan"), (R|"Asin"), (R|"Acos"), (R|"Atan"), (R|"Pi"), (R|"Sqrt"), (R|"Exp"), (R|"Log"), (R|"Log10"), (R|"Pow"), (R|"Compress"), (R|"Decompress"), (R|"Clear"));
}

void RefalInit_file9_IsUnimplemented() {
	file9_IsUnimplemented
	||
	(R|e_Name) & (R|R<Unimplemented>R) | (R|e_Foo, (R|e_Name), e_Bar)	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}

void RefalInit_file9_CheckBuiltin() {
	file9_CheckBuiltin
	||
	(R|e_Foo, (R|s_Number, Clear, s_Symbol))	^ (R|R<file9_ProcessBuiltin, e_Foo, (R|s_Number, Clear, s_Symbol)>R)
	||
	(R|e_Foo)	^ (R|R<file9_ProcessBuiltin, e_Foo>R, R<file9_NotInStandard>R);
}

void RefalInit_file9_ProcessBuiltin() {
	file9_ProcessBuiltin
	||
	(R|(R|s_Number, s_Name, s_Symbol), e_Bar) & (R|R<file9_IsUnimplemented, R<Explode, s_Name>R>R) | (R|True)	^ (R|R<file9_ProcessBuiltin, e_Bar>R)
	||
	(R|(R|s_Number, s_Name, s_Symbol), e_Bar)	^ (R|(R|R<Explode, s_Name>R), R<file9_ProcessBuiltin, e_Bar>R)
	||
	(~R)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"ProcessBuiltin\" function. View field: ", e_Foo>R);
}

void RefalInit_CheckSyntax() {
	CheckSyntax
	||
	(R|(~R))	^ (R|(~R))
	||
	(R|(R|t_FileName, (R|ErrorsList, "=", (~R)), e_FileCont))	^ (R|(R|t_FileName, e_FileCont))
	||
	(R|(R|t_FileName, (R|ErrorsList, "=", (R|e_ErrorsList)), e_FileCont))	^ (R|R<PrintErrors, (R|FileName, "=", t_FileName), e_ErrorsList>R, (~R))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"CheckSyntax\" function. View field: ", e_Foo>R);
}

void RefalInit_SyntaxDirectives() {
	SyntaxDirectives
	||
	(R|t_ErrorsList, e_Clause, (R|t_LineNumber, s_DelimType, Semicolon), e_Text)	^ (R|R<SyntaxDirectives, R<file11_SyntaxOne, t_ErrorsList, e_Clause, (R|t_LineNumber, R<GetDelimType>R, Semicolon)>R, e_Text>R)
	||
	(R|t_ErrorsList)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, e_Text)	^ (R|R<file11_SyntaxOne, t_ErrorsList, e_Text, (R|R<LastLine, e_Text>R, R<GetDelimType>R, Semicolon)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"SyntaxDirectives\" function. View field: ", e_Foo>R);
}

void RefalInit_file11_SyntaxOne() {
	file11_SyntaxOne
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "DECLARE_PUBLIC_FUNCTION"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_FunName), (R|s_LineNumber4, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "DECLARE_PUBLIC_SYMBOL"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_SymbName), (R|s_LineNumber4, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "DECLARE_EXTERNAL_FUNCTION"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_FunName), (R|s_LineNumber4, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "DECLARE_EXTERNAL_SYMBOL"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_SymbName), (R|s_LineNumber4, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "DECLARE_NAME"), (R|s_LineNumber2, s_SymbolType, e_Name), (R|s_LineNumber3, s_DelimType, Equal), (R|s_LineNumber4, s_SymbolType, e_Ident), (R|s_LineNumber5, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "DECLARE_NAME"), (R|s_LineNumber2, s_VarType, e_Name), (R|s_LineNumber3, s_DelimType, Equal), (R|s_LineNumber4, s_SymbolType, e_Ident), (R|s_LineNumber5, s_DelimType, Semicolon)) & (R|R<GetVarType>R) | (R|s_VarType) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "DECLARE_CHAR"), (R|s_LineNumber2, s_TextConstantType, s_Char), (R|s_LineNumber3, s_DelimType, Equal), (R|s_LineNumber4, s_SymbolType, e_Ident), (R|s_LineNumber5, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetTextConstantType>R) | (R|s_TextConstantType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "MODULE_NAME"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_ModuleName), (R|s_LineNumber4, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "CXX_FILE_PATH"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_TextConstantType, e_FilePath), (R|s_LineNumber4, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetTextConstantType>R) | (R|s_TextConstantType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "HXX_FILE_PATH"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_TextConstantType, e_FilePath), (R|s_LineNumber4, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetTextConstantType>R) | (R|s_TextConstantType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "DECLARE_USED_MODULE"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_TextConstantType, e_UsedModule), (R|s_LineNumber4, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetTextConstantType>R) | (R|s_TextConstantType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber1, s_SymbolType, "INIT_FUNCTION_NAME"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_FunName), (R|s_LineNumber4, s_DelimType, Semicolon)) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|t_ErrorsList)
	||
	(R|t_ErrorsList, (R|s_LineNumber, s_Type, e_Foo), e_Text)	^ (R|R<AddError, t_ErrorsList, (R|s_LineNumber, IrinaError_IllegalExpr, e_Foo, "...")>R)
	||
	(R|t_ErrorsList, (R|(R|s_LineNumber1, s_DelimType, s_LBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RBracket)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType)	^ (R|R<AddError, t_ErrorsList, (R|s_LineNumber1, IrinaError_IllegalExpr, s_LBracket, "...")>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"SyntaxOne\" function. View field: ", e_Foo>R);
}

void RefalInit_Linker() {
	Linker
	||
	(R|e_Foo, (~R), e_Bar)	^ (~R)
	||
	(R|(R|t_FileName, e_FileCont), e_Files)	^ (R|R<CheckLinker, R<MoreLinker, (R|ErrorsList, "=", (~R)), R<NotEmptyPublics, R<file12_GatherRest, (R|t_FileName, R<file12_GetDirectives, R<file12_MergeInFiles, e_Files, (R|t_FileName, e_FileCont)>R>R), (R|t_FileName, e_FileCont), e_Files>R>R>R, (R|t_FileName, e_FileCont), e_Files>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Linker\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_DirectivesTypesNumber() {
	file12_DirectivesTypesNumber
	||
	(~R)	^ (R|11);
}

void RefalInit_file12_ListsNumber() {
	file12_ListsNumber
	||
	(~R)	^ (R|4);
}

void RefalInit_NextNumbersFrom() {
	NextNumbersFrom
	||
	(~R)	^ (R|100);
}

void RefalInit_file12_GetDirectives() {
	file12_GetDirectives
	||
	(R|e_FileCont)	^ (R|R<file12_GetSomethingMore, (~R), R<file12_GetSomething, 1, (~R), e_FileCont>R>R);
}

void RefalInit_file12_MergeInFiles() {
	file12_MergeInFiles
	||
	(R|(R|t_FileName, e_FileCont), e_Files)	^ (R|R<file12_Merge, e_FileCont>R, R<file12_MergeInFiles, e_Files>R)
	||
	(~R)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"MergeInFiles\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_Merge() {
	file12_Merge
	||
	(R|(R|s_LineNumber2, s_DirectType), (R|(R|s_LineNumber3, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber, s_DelimType, Semicolon), (R|s_LineNumber4, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetDirectType>R) | (R|s_DirectType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|e_Foo, (R|s_LineNumber, s_DelimType, Semicolon), R<file12_Merge, e_Text>R)
	||
	(R|(R|s_LineNumber2, s_DirectType), (R|(R|s_LineNumber3, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber4, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetDirectType>R) | (R|s_DirectType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|e_Foo, (R|R<LastLine, e_Foo>R, R<GetDelimType>R, Semicolon), R<file12_Merge, e_Text>R)
	||
	(~R)	^ (~R)
	||
	(R|t_Foo, e_Text)	^ (R|R<file12_Merge, e_Text>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Merge\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GetSomething() {
	file12_GetSomething
	||
	(R|1, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "DECLARE_PUBLIC_FUNCTION"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_FunName), (R|s_LineNumber4, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 1, (R|e_Already, (R|s_LineNumber3, e_FunName)), e_Text>R)
	||
	(R|2, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "DECLARE_PUBLIC_SYMBOL"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_SymbName), (R|s_LineNumber4, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 2, (R|e_Already, (R|s_LineNumber3, e_SymbName)), e_Text>R)
	||
	(R|3, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "DECLARE_EXTERNAL_FUNCTION"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_FunName), (R|s_LineNumber4, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 3, (R|e_Already, (R|s_LineNumber3, e_FunName)), e_Text>R)
	||
	(R|4, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "DECLARE_EXTERNAL_SYMBOL"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_SymbName), (R|s_LineNumber4, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 4, (R|e_Already, (R|s_LineNumber3, e_SymbName)), e_Text>R)
	||
	(R|5, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "DECLARE_NAME"), (R|s_LineNumber2, s_SymbolType, e_Name), (R|s_LineNumber3, s_DelimType, Equal), (R|s_LineNumber4, s_SymbolType, e_Ident), (R|s_LineNumber5, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 5, (R|e_Already, (R|s_LineNumber4, (R|e_Name), (R|e_Ident))), e_Text>R)
	||
	(R|5, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "DECLARE_NAME"), (R|s_LineNumber2, s_VarType, e_Name), (R|s_LineNumber3, s_DelimType, Equal), (R|s_LineNumber4, s_SymbolType, e_Ident), (R|s_LineNumber5, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 5, (R|e_Already, (R|s_LineNumber4, (R|e_Name), (R|e_Ident))), e_Text>R)
	||
	(R|6, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "DECLARE_CHAR"), (R|s_LineNumber2, s_TextConstantType, s_Char), (R|s_LineNumber3, s_DelimType, Equal), (R|s_LineNumber4, s_SymbolType, e_Ident), (R|s_LineNumber5, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 6, (R|e_Already, (R|s_LineNumber4, (R|s_Char), (R|e_Ident))), e_Text>R)
	||
	(R|7, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "MODULE_NAME"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_TextConstantType, e_ModuleName), (R|s_LineNumber4, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 7, (R|e_ModuleName), e_Text>R)
	||
	(R|8, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "CXX_FILE_PATH"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_TextConstantType, e_FilePath), (R|s_LineNumber4, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 8, (R|e_FilePath), e_Text>R)
	||
	(R|9, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "HXX_FILE_PATH"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_TextConstantType, e_FilePath), (R|s_LineNumber4, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 9, (R|e_FilePath), e_Text>R)
	||
	(R|10, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "DECLARE_USED_MODULE"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_TextConstantType, e_UsedModule), (R|s_LineNumber4, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 10, (R|e_Already, (R|s_LineNumber3, e_UsedModule)), e_Text>R)
	||
	(R|11, (R|e_Already), (R|s_LineNumber1, s_SymbolType, "INIT_FUNCTION_NAME"), (R|s_LineNumber2, s_DelimType, Equal), (R|s_LineNumber3, s_SymbolType, e_FunName), (R|s_LineNumber4, s_DelimType, Semicolon), e_Text)	^ (R|R<file12_GetSomething, 11, (R|e_FunName), e_Text>R)
	||
	(R|s_N, (R|e_Already), t_Foo, e_Text)	^ (R|t_Foo, R<file12_GetSomething, s_N, (R|e_Already), e_Text>R)
	||
	(R|s_N, (R|e_Already))	^ (R|s_N, (R|e_Already))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GetSomething\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GetSomethingMore() {
	file12_GetSomethingMore
	||
	(R|(R|e_Got), e_Text, s_N, (R|e_Already)) & (R|R<file12_DirectivesTypesNumber>R) | (R|s_N)	^ (R|(R|e_Got, (R|s_N, e_Already)))
	||
	(R|(R|e_Got), e_Text, 6, (R|e_Already))	^ (R|R<file12_GetSomethingMore, (R|e_Got, (R|6, e_Already)), R<file12_GetSomething, 7, (R|"refalpart"), e_Text>R>R)
	||
	(R|(R|e_Got), e_Text, 10, (R|e_Already))	^ (R|R<file12_GetSomethingMore, (R|e_Got, (R|10, e_Already)), R<file12_GetSomething, 11, (R|"RefalInit"), e_Text>R>R)
	||
	(R|(R|e_Got), e_Text, s_N, (R|e_Already))	^ (R|R<file12_GetSomethingMore, (R|e_Got, (R|s_N, e_Already)), R<file12_GetSomething, R<Add, s_N, 1>R, (~R), e_Text>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GetSomethingMore\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GatherRest() {
	file12_GatherRest
	||
	(R|(R|t_FileName, t_Got), e_Files)	^ (R|R<file12_GatherMore, (R|t_FileName, t_Got), R<file12_Gather, 1, e_Files>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GatherRest\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_Gather() {
	file12_Gather
	||
	(R|s_N, e_Files)	^ (R|e_Files, R<file12_GatherInFiles, s_N, (~R), e_Files>R, s_N)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Gather\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GatherMore() {
	file12_GatherMore
	||
	(R|(R|t_FileName, (R|e_Got)), e_Files, (R|e_Already), s_N) & (R|R<file12_ListsNumber>R) | (R|s_N)	^ (R|(R|t_FileName, (R|e_Got, (R|R<Add, R<NextNumbersFrom>R, s_N>R, e_Already))))
	||
	(R|(R|t_FileName, (R|e_Got)), e_Files, (R|e_Already), s_N)	^ (R|R<file12_GatherMore, (R|t_FileName, (R|e_Got, (R|R<Add, R<NextNumbersFrom>R, s_N>R, e_Already))), R<file12_Gather, R<Add, s_N, 1>R, e_Files>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GatherMore\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GatherInFiles() {
	file12_GatherInFiles
	||
	(R|s_N, (R|e_Already), t_File, e_Files)	^ (R|R<file12_GatherInFiles, s_N, R<file12_GatherInOneFile, s_N, (R|e_Already), t_File>R, e_Files>R)
	||
	(R|s_N, (R|e_Already))	^ (R|(R|e_Already))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GatherInFiles\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GatherInOneFile() {
	file12_GatherInOneFile
	||
	(R|1, (R|e_Already), (R|t_FileName, e_FileCont))	^ (R|R<file12_GatherVars, (R|e_Already), e_FileCont>R)
	||
	(R|2, (R|e_Already), (R|t_FileName, e_FileCont))	^ (R|R<file12_GatherSymbols, (R|e_Already), e_FileCont>R)
	||
	(R|3, (R|e_Already), (R|t_FileName, e_FileCont))	^ (R|R<file12_GatherEntries, (R|e_Already), e_FileCont>R)
	||
	(R|4, (R|e_Already), (R|t_FileName, e_FileCont))	^ (R|R<file12_GatherExterns, (R|e_Already), e_FileCont>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GatherInOneFile\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GatherVars() {
	file12_GatherVars
	||
	(R|(R|e_Already), (R|s_LineNumber, s_VarType, e_Var), e_Text) & (R|R<GetVarType>R) | (R|s_VarType)	^ (R|R<file12_GatherVars, R<file12_AddVarOrSymbol, (R|e_Already), (R|e_Var)>R, e_Text>R)
	||
	(R|(R|e_Already), (R|(R|s_LineNumber1, s_DelimType, s_LBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RBracket)), e_Text) & (R|R<GetDelimType>R) | (R|s_DelimType)	^ (R|R<file12_GatherVars, R<file12_GatherVars, (R|e_Already), e_Foo>R, e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber, s_Type, e_Foo), e_Text)	^ (R|R<file12_GatherVars, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already))	^ (R|(R|e_Already))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GatherVars\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GatherSymbols() {
	file12_GatherSymbols
	||
	(R|(R|e_Already), (R|s_LineNumber0, s_ExternType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|s_LineNumber2, s_DelimType, Semicolon), e_Text) & (R|R<GetExternType>R) | (R|s_ExternType) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|R<file12_GatherSymbols, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber0, s_EntryType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetEntryType>R) | (R|s_EntryType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file12_GatherSymbols, R<file12_GatherSymbols, (R|e_Already), e_FunCont>R, e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber0, s_DirectivesType), (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetDirectType>R) | (R|s_DirectivesType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file12_GatherSymbols, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file12_GatherSymbols, R<file12_GatherSymbols, (R|e_Already), e_FunCont>R, e_Text>R)
	||
	(R|(R|e_Already), (R|(R|s_LineNumber1, s_DelimType, s_LFBracket), (R|s_LineNumber2, s_SymbolType, e_Symbol), e_FunArgs, (R|s_LineNumber3, s_DelimType, s_RFBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "<">R) | (R|s_LFBracket) & (R|R<GetDelimCode, ">">R) | (R|s_RFBracket)	^ (R|R<file12_GatherSymbols, R<file12_GatherSymbols, (R|e_Already), e_FunArgs>R, e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber, s_SymbolType, e_Symbol), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|R<file12_GatherSymbols, R<file12_AddVarOrSymbol, (R|e_Already), (R|e_Symbol)>R, e_Text>R)
	||
	(R|(R|e_Already), (R|(R|s_LineNumber1, s_DelimType, s_LBracket), e_Foo, (R|s_LineNumber3, s_DelimType, s_RBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimType>R) | (R|s_DelimType)	^ (R|R<file12_GatherSymbols, R<file12_GatherSymbols, (R|e_Already), e_Foo>R, e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber, s_Type, e_Foo), e_Text)	^ (R|R<file12_GatherSymbols, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already))	^ (R|(R|e_Already))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GatherSymbols\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GatherEntries() {
	file12_GatherEntries
	||
	(R|(R|e_Already), (R|s_LineNumber0, s_ExternType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|s_LineNumber2, s_DelimType, Semicolon), e_Text) & (R|R<GetExternType>R) | (R|s_ExternType) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|R<file12_GatherEntries, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber0, s_EntryType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetEntryType>R) | (R|s_EntryType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file12_GatherEntries, R<file12_AddEntry, (R|e_Already), (R|e_Symbol)>R, e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber0, s_DirectivesType), (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetDirectType>R) | (R|s_DirectivesType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file12_GatherEntries, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file12_GatherEntries, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already))	^ (R|(R|e_Already))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GatherEntries\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_GatherExterns() {
	file12_GatherExterns
	||
	(R|(R|e_Already), (R|s_LineNumber0, s_ExternType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|s_LineNumber2, s_DelimType, Semicolon), e_Text) & (R|R<GetExternType>R) | (R|s_ExternType) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|R<file12_GatherExterns, R<file12_AddVarOrSymbol, (R|e_Already), (R|e_Symbol)>R, e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber0, s_EntryType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetEntryType>R) | (R|s_EntryType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file12_GatherExterns, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber0, s_DirectivesType), (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetDirectType>R) | (R|s_DirectivesType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file12_GatherExterns, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file12_GatherExterns, (R|e_Already), e_Text>R)
	||
	(R|(R|e_Already))	^ (R|(R|e_Already))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GatherExterns\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_AddVarOrSymbol() {
	file12_AddVarOrSymbol
	||
	(R|(R|e_Foo, (R|e_Var), e_Bar), (R|e_Var))	^ (R|(R|e_Foo, (R|e_Var), e_Bar))
	||
	(R|(R|e_Foo), (R|e_Var))	^ (R|(R|e_Foo, (R|e_Var)))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddVarOrSymbol\" function. View field: ", e_Foo>R);
}

void RefalInit_file12_AddEntry() {
	file12_AddEntry
	||
	(R|(R|e_Foo, (R|s_Number, e_Fun), e_Bar), (R|e_Fun))	^ (R|(R|e_Foo, (R|R<Add, s_Number, 1>R, e_Fun), e_Bar))
	||
	(R|(R|e_Foo), (R|e_Fun))	^ (R|(R|e_Foo, (R|0, e_Fun)))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddEntry\" function. View field: ", e_Foo>R);
}

void RefalInit_MoreLinker() {
	MoreLinker
	||
	(R|t_Errors, t_Directives)	^ (R|R<file13_MoreChecks, t_Errors, t_Directives, R<file13_MakeCheck, 1, (R|ErrorsList, "=", (~R)), t_Directives>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"MoreLinker\" function. View field: ", e_Foo>R);
}

void RefalInit_file13_MakeCheck() {
	file13_MakeCheck
	||
	(R|1, t_Errors, (R|t_Name, (R|e_Foo, (R|s_N, e_Entries), e_Bar))) & (R|R<Add, R<NextNumbersFrom>R, 3>R) | (R|s_N)	^ (R|1, R<file13_NoDupesInEntries, t_Errors, e_Entries>R)
	||
	(R|2, t_Errors, (R|t_Name, (R|e_Foo, (R|3, e_ExternsDeclared), e_Bar1, (R|s_N1, e_Entries), e_Bar2, (R|s_N2, e_Externs), e_Bar3))) & (R|R<Add, R<NextNumbersFrom>R, 3>R) | (R|s_N1) & (R|R<Add, R<NextNumbersFrom>R, 4>R) | (R|s_N2)	^ (R|2, R<file13_NoUnresolvedExternals, t_Errors, (R|e_Externs), (R|e_Entries), (R|e_ExternsDeclared)>R)
	||
	(R|3, t_Errors, (R|t_Name, (R|e_Foo, (R|1, e_PublicsDeclared), e_Bar1, (R|s_N1, e_Entries), e_Bar2))) & (R|R<Add, R<NextNumbersFrom>R, 3>R) | (R|s_N1)	^ (R|3, R<file13_EveryPublicIsEntry, t_Errors, (R|e_PublicsDeclared), (R|e_Entries)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"MakeCheck\" function. View field: ", e_Foo>R);
}

void RefalInit_file13_ChecksNumber() {
	file13_ChecksNumber
	||
	(~R)	^ (R|3);
}

void RefalInit_NotEmptyPublics() {
	NotEmptyPublics
	||
	(R|(R|t_Name, (R|e_Foo, (R|1), e_Bar)))	^ (R|(R|t_Name, (R|e_Foo, (R|1, (R|0, "Go")), e_Bar)))
	||
	(R|e_Foo)	^ (R|e_Foo);
}

void RefalInit_file13_MoreChecks() {
	file13_MoreChecks
	||
	(R|(R|ErrorsList, "=", (R|e_Already)), t_Directives, s_N, (R|e_NewErrors)) & (R|R<file13_ChecksNumber>R) | (R|s_N)	^ (R|(R|ErrorsList, "=", (R|e_Already, e_NewErrors)), t_Directives)
	||
	(R|(R|ErrorsList, "=", (R|e_Already)), t_Directives, s_N, (R|e_NewErrors))	^ (R|R<file13_MoreChecks, (R|ErrorsList, "=", (R|e_Already, e_NewErrors)), t_Directives, R<file13_MakeCheck, R<Add, s_N, 1>R, (R|ErrorsList, "=", (~R)), t_Directives>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"MoreChecks\" function. View field: ", e_Foo>R);
}

void RefalInit_file13_NoDupesInEntries() {
	file13_NoDupesInEntries
	||
	(R|t_ErrorsList, (R|0, e_Fun), e_Text)	^ (R|R<file13_NoDupesInEntries, t_ErrorsList, e_Text>R)
	||
	(R|t_ErrorsList, (R|s_N, e_Fun), e_Text)	^ (R|R<file13_NoDupesInEntries, R<AddError, t_ErrorsList, (R|0, IrinaError_EntryRedefinition, e_Fun)>R, e_Text>R)
	||
	(R|(R|ErrorsList, "=", (R|e_Already)))	^ (R|(R|e_Already))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"NoDupesInEntries\" function. View field: ", e_Foo>R);
}

void RefalInit_file13_NoUnresolvedExternals() {
	file13_NoUnresolvedExternals
	||
	(R|(R|ErrorsList, "=", (R|e_Already)), (~R), (R|e_Entries), (R|e_ExternsDeclared))	^ (R|(R|e_Already))
	||
	(R|t_Errors, (R|(R|e_Fun), e_Bar), (R|e_Foo2, (R|s_Number, e_Fun), e_Bar2), (R|e_ExternsDeclared))	^ (R|R<file13_NoUnresolvedExternals, t_Errors, (R|e_Bar), (R|e_Foo2, (R|s_Number, e_Fun), e_Bar2), (R|e_ExternsDeclared)>R)
	||
	(R|t_Errors, (R|(R|e_Fun), e_Bar), (R|e_Entries), (R|e_Foo2, (R|s_LineNumber, e_Fun), e_Bar2))	^ (R|R<file13_NoUnresolvedExternals, t_Errors, (R|e_Bar), (R|e_Entries), (R|e_Foo2, (R|s_LineNumber, e_Fun), e_Bar2)>R)
	||
	(R|t_Errors, (R|(R|e_Fun), e_Bar), (R|e_Entries), (R|e_ExternsDeclared))	^ (R|R<file13_NoUnresolvedExternals, R<AddError, t_Errors, (R|0, IrinaError_UnresolvedExternal, e_Fun)>R, (R|e_Bar), (R|e_Entries), (R|e_ExternsDeclared)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"NoUnresolvedExternals\" function. View field: ", e_Foo>R);
}

void RefalInit_file13_EveryPublicIsEntry() {
	file13_EveryPublicIsEntry
	||
	(R|(R|ErrorsList, "=", (R|e_Already)), (~R), (R|e_Entries))	^ (R|(R|e_Already))
	||
	(R|t_Errors, (R|(R|s_LineNumber, e_Fun), e_Text), (R|e_Foo, (R|s_Number, e_Fun), e_Bar))	^ (R|R<file13_EveryPublicIsEntry, t_Errors, (R|e_Text), (R|e_Foo, (R|s_Number, e_Fun), e_Bar)>R)
	||
	(R|t_Errors, (R|(R|s_LineNumber, e_Fun), e_Text), (R|e_Entries))	^ (R|R<file13_EveryPublicIsEntry, R<AddError, t_Errors, (R|0, IrinaError_PublicIsNotEntry, e_Fun)>R, (R|e_Text), (R|e_Entries)>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"EveryPublicIsEntry\" function. View field: ", e_Foo>R);
}

void RefalInit_CheckLinker() {
	CheckLinker
	||
	(R|(R|ErrorsList, "=", (~R)), t_Directives, e_Files)	^ (R|R<Generate, t_Directives, e_Files>R)
	||
	(R|(R|ErrorsList, "=", (R|e_ErrorsList)), t_Directives, e_Files)	^ (R|R<PrintErrors, LinkerErrors, e_ErrorsList>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"CheckLinker\" function. View field: ", e_Foo>R);
}

void RefalInit_Generate() {
	Generate
	||
	(R|t_Directives, e_Files)	^ (R|R<GenerateHxx, t_Directives>R, R<GenerateCxx, t_Directives, e_Files>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Generate\" function. View field: ", e_Foo>R);
}

void RefalInit_GenerateName() {
	GenerateName
	||
	(R|(R|e_Foo, (R|s_LineNumber, (R|e_Name1), (R|e_Name2)), e_Bar), (R|e_CharsConvention), (R|e_Name1))	^ (R|e_Name2)
	||
	(R|(R|e_NamesConvention), (R|e_Foo, (R|s_LineNumber, (R|s_Char), (R|e_Name)), e_Bar), (R|s_Char, e_Rest))	^ (R|e_Name, R<GenerateName, (R|e_NamesConvention), (R|e_Foo, (R|s_LineNumber, (R|s_Char), (R|e_Name)), e_Bar), (R|e_Rest)>R)
	||
	(R|(R|e_NamesConvention), (R|e_CharsConvention), (R|s_Char, e_Rest)) & (R|R<IsLetter, s_Char>R) | (R|True)	^ (R|s_Char, R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Rest)>R)
	||
	(R|(R|e_NamesConvention), (R|e_CharsConvention), (R|s_Char, e_Rest)) & (R|R<IsDigit, s_Char>R) | (R|True)	^ (R|s_Char, R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Rest)>R)
	||
	(R|(R|e_NamesConvention), (R|e_CharsConvention), (R|s_Char, e_Rest))	^ (R|"_", R<Symb, R<Ord, s_Char>R>R, "_", R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Rest)>R)
	||
	(R|(R|e_NamesConvention), (R|e_CharsConvention), (~R))	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GenerateName\" function. View field: ", e_Foo>R);
}

void RefalInit_AddSymbol() {
	AddSymbol
	||
	(R|(R|(R|s_LineNumber, e_Fun), e_Text), (R|e_NamesConvention), (R|e_CharsConvention))	^ (R|"extern RfSymbol ", R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Fun)>R, ";\n", R<AddSymbol, (R|e_Text), (R|e_NamesConvention), (R|e_CharsConvention)>R)
	||
	(R|(~R), (R|e_NamesConvention), (R|e_CharsConvention))	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddSymbol\" function. View field: ", e_Foo>R);
}

void RefalInit_TwoNewlines() {
	TwoNewlines
	||
	(~R)	^ (R|"\n", "\n");
}

void RefalInit_IsStatic() {
	IsStatic
	||
	(R|(R|e_Ident), (R|e_Foo, (R|s_LineNumber, e_Ident), e_Bar))	^ (R|"       ")
	||
	(R|e_Foo)	^ (R|"static ");
}

void RefalInit_GenerateHxx() {
	GenerateHxx
	||
	(R|t_Directives)	^ (R|(R|(R|R<file16_GetHxxName, t_Directives>R), (R|R<file16_GetHxxCont, t_Directives>R)))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GenerateHxx\" function. View field: ", e_Foo>R);
}

void RefalInit_file16_GetHxxName() {
	file16_GetHxxName
	||
	(R|(R|t_Name, (R|e_Foo, (R|7, e_ModuleName), e_Bar1, (R|9), e_Bar2)))	^ (R|e_ModuleName, ".hxx")
	||
	(R|(R|t_Name, (R|e_Foo, (R|7, e_ModuleName), e_Bar1, (R|9, e_HxxFileName), e_Bar2)))	^ (R|e_HxxFileName)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GetHxxName\" function. View field: ", e_Foo>R);
}

void RefalInit_file16_HxxPartsNumber() {
	file16_HxxPartsNumber
	||
	(~R)	^ (R|4);
}

void RefalInit_file16_GetHxxCont() {
	file16_GetHxxCont
	||
	(R|t_Directives)	^ (R|R<file16_FillHxx, 1, t_Directives, R<file16_StartFilling, 1, t_Directives>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GetHxxCont\" function. View field: ", e_Foo>R);
}

void RefalInit_file16_FillHxx() {
	file16_FillHxx
	||
	(R|s_N, t_Directives) & (R|R<file16_HxxPartsNumber>R) | (R|s_N)	^ (~R)
	||
	(R|s_N, t_Directives, e_Text) & (R|R<file16_HxxPartsNumber>R) | (R|s_N)	^ (R|R<TwoNewlines>R, e_Text)
	||
	(R|s_N, t_Directives)	^ (R|R<file16_FillHxx, R<Add, s_N, 1>R, t_Directives, R<file16_StartFilling, R<Add, s_N, 1>R, t_Directives>R>R)
	||
	(R|s_N, t_Directives, e_Text)	^ (R|R<TwoNewlines>R, e_Text, R<file16_FillHxx, R<Add, s_N, 1>R, t_Directives, R<file16_StartFilling, R<Add, s_N, 1>R, t_Directives>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"FillHxx\" function. View field: ", e_Foo>R);
}

void RefalInit_file16_StartFilling() {
	file16_StartFilling
	||
	(R|1, t_Directives)	^ (R|R<file16_AddHeader>R)
	||
	(R|2, (R|t_Name, (R|e_Foo, (R|1, e_PublicFunctions), e_Bar1, (R|5, e_NamesConvention), e_Bar2, (R|6, e_CharsConvention), e_Bar3)))	^ (R|R<AddSymbol, (R|e_PublicFunctions), (R|e_NamesConvention), (R|e_CharsConvention)>R)
	||
	(R|3, (R|t_Name, (R|e_Foo, (R|2, e_PublicSymbols), e_Bar1, (R|5, e_NamesConvention), e_Bar2, (R|6, e_CharsConvention), e_Bar3)))	^ (R|R<AddSymbol, (R|e_PublicSymbols), (R|e_NamesConvention), (R|e_CharsConvention)>R)
	||
	(R|4, (R|t_Name, (R|e_Foo, (R|11, e_InitFunction), e_Bar)))	^ (R|R<file16_AddInitFunction, e_InitFunction>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StartFilling\" function. View field: ", e_Foo>R);
}

void RefalInit_file16_AddHeader() {
	file16_AddHeader
	||
	(~R)	^ (R|"#ifndef INTELIB_REFAL_TRANSLATED_HXX\n", "#define INTELIB_REFAL_TRANSLATED_HXX\n", "\n", "#include \"sexpress/sexpress.hpp\"\n", "\n", "#include \"refal/rclause.hpp\"\n");
}

void RefalInit_file16_AddInitFunction() {
	file16_AddInitFunction
	||
	(R|e_InitFunction)	^ (R|"void ", e_InitFunction, "();\n", "void ", e_InitFunction, "(int argc, char **argv);\n", "void RefalFree();\n", "#endif\n");
}

void RefalInit_GenerateCxx() {
	GenerateCxx
	||
	(R|t_Directives, e_Files)	^ (R|(R|(R|R<file17_GetCxxName, t_Directives>R), (R|R<file17_GetCxxCont, t_Directives, e_Files>R)))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GenerateCxx\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_GetCxxName() {
	file17_GetCxxName
	||
	(R|(R|t_Name, (R|e_Foo, (R|7, e_ModuleName), e_Bar1, (R|8), e_Bar2)))	^ (R|e_ModuleName, ".cxx")
	||
	(R|(R|t_Name, (R|e_Foo, (R|7, e_ModuleName), e_Bar1, (R|8, e_CxxFileName), e_Bar2)))	^ (R|e_CxxFileName)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GetCxxName\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_CxxPartsNumber() {
	file17_CxxPartsNumber
	||
	(~R)	^ (R|7);
}

void RefalInit_file17_GetCxxCont() {
	file17_GetCxxCont
	||
	(R|t_Directives, e_Files)	^ (R|R<file17_FillCxx, 1, t_Directives, (R|e_Files), R<file17_StartFilling, 1, t_Directives>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GetCxxCont\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_FillCxx() {
	file17_FillCxx
	||
	(R|s_N, t_Directives, (R|e_Files)) & (R|R<file17_CxxPartsNumber>R) | (R|s_N)	^ (~R)
	||
	(R|s_N, t_Directives, (R|e_Files), e_Text) & (R|R<file17_CxxPartsNumber>R) | (R|s_N)	^ (R|R<TwoNewlines>R, e_Text)
	||
	(R|6, t_Directives, (R|e_Files))	^ (R|R<file17_FillCxx, 7, t_Directives, (R|e_Files), R<file17_StartFilling, 7, t_Directives, e_Files>R>R)
	||
	(R|6, t_Directives, (R|e_Files), e_Text)	^ (R|R<TwoNewlines>R, e_Text, R<file17_FillCxx, 7, t_Directives, (R|e_Files), R<file17_StartFilling, 7, t_Directives, e_Files>R>R)
	||
	(R|s_N, t_Directives, (R|e_Files))	^ (R|R<file17_FillCxx, R<Add, s_N, 1>R, t_Directives, (R|e_Files), R<file17_StartFilling, R<Add, s_N, 1>R, t_Directives>R>R)
	||
	(R|s_N, t_Directives, (R|e_Files), e_Text)	^ (R|R<TwoNewlines>R, e_Text, R<file17_FillCxx, R<Add, s_N, 1>R, t_Directives, (R|e_Files), R<file17_StartFilling, R<Add, s_N, 1>R, t_Directives>R>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"FillCxx\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_StartFilling() {
	file17_StartFilling
	||
	(R|1, (R|t_Name, (R|e_Foo, (R|10, e_UsedModules), e_Bar)))	^ (R|R<file17_AddHeader, e_UsedModules>R)
	||
	(R|2, (R|t_Name, (R|e_Foo, (R|3, e_ExternalFunctions), e_Bar1, (R|5, e_NamesConvention), e_Bar2, (R|6, e_CharsConvention), e_Bar3)))	^ (R|R<AddSymbol, (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention)>R)
	||
	(R|3, t_Directives)	^ (R|R<file17_AddLibrary, R<Library>R>R)
	||
	(R|4, (R|t_Name, (R|e_Foo, (R|4, e_ExternalSymbols), e_Bar1, (R|5, e_NamesConvention), e_Bar2, (R|6, e_CharsConvention), e_Bar3)))	^ (R|R<AddSymbol, (R|e_ExternalSymbols), (R|e_NamesConvention), (R|e_CharsConvention)>R)
	||
	(R|5, (R|t_Name, (R|e_Foo, (R|2, e_PublicSymbols), e_Bar1, (R|4, e_ExternalSymbols), e_Bar2, (R|5, e_NamesConvention), e_Bar3, (R|6, e_CharsConvention), e_Bar4, (R|s_N1, e_Symbols), e_Bar5, (R|s_N2, e_Entries), e_Bar6))) & (R|R<Add, R<NextNumbersFrom>R, 2>R) | (R|s_N1) & (R|R<Add, R<NextNumbersFrom>R, 3>R) | (R|s_N2)	^ (R|R<file17_AddAllSymbols, (R|e_Symbols), (R|e_PublicSymbols), (R|e_ExternalSymbols), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Entries), (R|R<Library>R)>R)
	||
	(R|6, (R|t_Name, (R|e_Foo, (R|5, e_NamesConvention), e_Bar1, (R|6, e_CharsConvention), e_Bar2, (R|s_N, e_Vars), e_Bar3))) & (R|R<Add, R<NextNumbersFrom>R, 1>R) | (R|s_N)	^ (R|R<file17_AddAllVariables, (R|e_Vars), (R|e_NamesConvention), (R|e_CharsConvention)>R)
	||
	(R|7, (R|t_Name, (R|e_Foo, (R|5, e_NamesConvention), e_Bar1, (R|6, e_CharsConvention), e_Bar2, (R|11, e_InitFunctionName), e_Bar3, (R|s_N1, e_PublicFunctions), e_Bar4, (R|s_N2, e_ExternalFunctions), e_Bar5)), e_Files) & (R|R<Add, R<NextNumbersFrom>R, 3>R) | (R|s_N1) & (R|R<Add, R<NextNumbersFrom>R, 4>R) | (R|s_N2)	^ (R|R<AddFunctionsCode, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_InitFunctionName), e_Files>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StartFilling\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_AddHeader() {
	file17_AddHeader
	||
	(R|e_UsedModules)	^ (R|"#include \"sexpress/iexcept.hpp\"\n", "#include \"sexpress/sexpress.hpp\"\n", "\n", "#include \"refal/refal.hpp\"\n", "#include \"refal/rclause.hpp\"\n", "#include \"refal/rvars.hpp\"\n", R<file17_AddUsedModules, e_UsedModules>R, "\n", "static RfFormConstructor R;\n", "\n", "RfWith RWITH;\n");
}

void RefalInit_file17_AddUsedModules() {
	file17_AddUsedModules
	||
	(R|(R|s_LineNumber, e_UsedModule), e_Text)	^ (R|"#include \"", e_UsedModule, "\"\n", R<file17_AddUsedModules, e_Text>R)
	||
	(~R)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddUsedModules\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_AddLibrary() {
	file17_AddLibrary
	||
	(R|(R|e_LibraryFunction), e_Text)	^ (R|"extern RfLibSymbol ", e_LibraryFunction, ";\n", R<file17_AddLibrary, e_Text>R)
	||
	(~R)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddLibrary\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_AddAllSymbols() {
	file17_AddAllSymbols
	||
	(R|(R|(R|e_Symbol), e_Text), (R|e_PublicSymbols), (R|e_Foo, (R|s_LineNumber, e_Symbol), e_Bar), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Entries), (R|e_Library))	^ (R|R<file17_AddAllSymbols, (R|e_Text), (R|e_PublicSymbols), (R|e_Foo, (R|e_Symbol), e_Bar), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Entries), (R|e_Library)>R)
	||
	(R|(R|(R|e_Symbol), e_Text), (R|e_PublicSymbols), (R|e_ExternalSymbols), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Entries), (R|e_Foo, (R|e_Symbol), e_Bar))	^ (R|R<file17_AddAllSymbols, (R|e_Text), (R|e_PublicSymbols), (R|e_ExternalSymbols), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Entries), (R|e_Foo, (R|e_Symbol), e_Bar)>R)
	||
	(R|(R|(R|e_Symbol), e_Text), (R|e_PublicSymbols), (R|e_ExternalSymbols), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Foo, (R|s_Number, e_Symbol), e_Bar), (R|e_Library))	^ (R|R<file17_AddAllSymbols, (R|e_Text), (R|e_PublicSymbols), (R|e_ExternalSymbols), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Foo, (R|s_Number, e_Symbol), e_Bar), (R|e_Library)>R)
	||
	(R|(R|(R|e_Symbol), e_Text), (R|e_PublicSymbols), (R|e_ExternalSymbols), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Entries), (R|e_Library))	^ (R|R<IsStatic, (R|e_Symbol), (R|e_PublicSymbols)>R, "RfSymbol ", R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Symbol)>R, "(\"", e_Symbol, "\");\n", R<file17_AddAllSymbols, (R|e_Text), (R|e_PublicSymbols), (R|e_ExternalSymbols), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Entries), (R|e_Library)>R)
	||
	(R|(~R), (R|e_PublicSymbols), (R|e_ExternalSymbols), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Entries), (R|e_Library))	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddAllSymbols\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_AddAllVariables() {
	file17_AddAllVariables
	||
	(R|(R|(R|e_Var), e_Text), (R|e_NamesConvention), (R|e_CharsConvention))	^ (R|"static RfVariable_", R<file17_GetPrefix, e_Var>R, " ", R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Var)>R, "(\"", e_Var, "\");\n", R<file17_AddAllVariables, (R|e_Text), (R|e_NamesConvention), (R|e_CharsConvention)>R)
	||
	(R|(~R), (R|e_NamesConvention), (R|e_CharsConvention))	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddAllVariables\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_GetPrefix() {
	file17_GetPrefix
	||
	(R|e_Prefix, ".", e_Suffix)	^ (R|R<file17_ToUpper, e_Prefix>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GetPrefix\" function. View field: ", e_Foo>R);
}

void RefalInit_file17_ToUpper() {
	file17_ToUpper
	||
	(R|"s")	^ (R|"S")
	||
	(R|"e")	^ (R|"E")
	||
	(R|"t")	^ (R|"T")
	||
	(R|e_Foo)	^ (R|e_Foo);
}

void RefalInit_file18_FirstFileNumber() {
	file18_FirstFileNumber
	||
	(~R)	^ (R|1);
}

void RefalInit_AddFunctionsCode() {
	AddFunctionsCode
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_InitFunctionName), e_Files)	^ (R|R<file18_FinishAdding, (R|e_InitFunctionName), R<file18_StartAdding, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (~R), (~R), (~R), R<file18_FirstFileNumber>R, e_Files>R>R, R<file18_GenerateInitFunctionArgc, e_InitFunctionName>R, R<file18_GenerateRefalFree>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddFunctionsCode\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_StartAdding() {
	file18_StartAdding
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber)	^ (R|(R|e_Already1), (R|e_Already2), (R|e_Already3))
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, (R|t_FileName, e_FileCont), e_Files)	^ (R|R<file18_StartAdding, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), R<file18_AddingFromOneFile, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, e_FileCont>R, R<Add, s_FileNumber, 1>R, e_Files>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"StartAdding\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_AddingFromOneFile() {
	file18_AddingFromOneFile
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, (R|s_LineNumber0, s_ExternType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|s_LineNumber2, s_DelimType, Semicolon), e_Text) & (R|R<GetExternType>R) | (R|s_ExternType) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|R<file18_AddingFromOneFile, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, e_Text>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, (R|s_LineNumber0, s_EntryType), (R|s_LineNumber1, s_SymbolType, e_Ident), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetEntryType>R) | (R|s_EntryType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file18_AddingFromOneFile, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), R<file18_AddToList, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, (R|e_Ident), (R|R<file18_GenerateFunCont, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|e_Ident), e_FunCont>R)>R, s_FileNumber, e_Text>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, (R|s_LineNumber0, s_DirectivesType), (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetDirectType>R) | (R|s_DirectivesType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file18_AddingFromOneFile, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, e_Text>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, (R|s_LineNumber1, s_SymbolType, e_Ident), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket)	^ (R|R<file18_AddingFromOneFile, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), R<file18_AddToList, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, (R|e_Ident), (R|R<file18_GenerateFunCont, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|e_Ident), e_FunCont>R)>R, s_FileNumber, e_Text>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber)	^ (R|(R|e_Already1), (R|e_Already2), (R|e_Already3))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddingFromOneFile\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_NeedsPrefix() {
	file18_NeedsPrefix
	||
	(R|(R|e_Ident), (R|e_Foo, (R|s_LineNumber, e_Ident), e_Bar), (R|e_ExternalFunctions), s_FileNumber)	^ (~R)
	||
	(R|(R|e_Ident), (R|e_PublicFunctions), (R|e_Foo, (R|s_LineNumber, e_Ident), e_Bar), s_FileNumber)	^ (~R)
	||
	(R|(R|e_Ident), (R|e_PublicFunctions), (R|e_ExternalFunctions), s_FileNumber) & (R|R<Library>R) | (R|e_Foo, (R|e_Ident), e_Bar)	^ (~R)
	||
	(R|(R|e_Ident), (R|e_PublicFunctions), (R|e_ExternalFunctions), s_FileNumber)	^ (R|"file", R<Symb, s_FileNumber>R, "_")
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"NeedsPrefix\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_AddToList() {
	file18_AddToList
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Already1), (R|e_Already2), (R|e_Already3), s_FileNumber, (R|e_Name), (R|e_FunCont))	^ (R|(R|e_Already1, R<IsStatic, (R|e_Name), (R|e_PublicFunctions)>R, "RfSymbol ", R<file18_NeedsPrefix, (R|e_Name), (R|e_PublicFunctions), (R|e_ExternalFunctions), s_FileNumber>R, R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Name)>R, "(\"", e_Name, "\");\n"), (R|e_Already2, (R|"void ", R<file18_NeedsPrefix, (R|e_Name), (R|e_PublicFunctions), (R|e_ExternalFunctions), s_FileNumber>R, R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Name)>R, "() {\n", e_FunCont, ";\n", "}\n")), (R|e_Already3, (R|R<file18_NeedsPrefix, (R|e_Name), (R|e_PublicFunctions), (R|e_ExternalFunctions), s_FileNumber>R, R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Name)>R, "();\n")))
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddToList\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_FinishAdding() {
	file18_FinishAdding
	||
	(R|(R|e_InitFunctionName), (R|e_Already1), (R|e_Already2), (R|e_Already3))	^ (R|e_Already1, R<TwoNewlines>R, R<file18_AddInitFunction, (R|e_InitFunctionName), e_Already2>R, "\n", "void ", e_InitFunctionName, "() {\n", R<file18_MakeInitFunction, (R|e_InitFunctionName), e_Already3>R, "}\n")
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"FinishAdding\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_AddInitFunction() {
	file18_AddInitFunction
	||
	(R|(R|e_InitFunctionName), (R|"void ", e_Foo), e_Text)	^ (R|"void ", e_InitFunctionName, "_", e_Foo, "\n", R<file18_AddInitFunction, (R|e_InitFunctionName), e_Text>R)
	||
	(R|(R|e_InitFunctionName))	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"AddInitFunction\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_MakeInitFunction() {
	file18_MakeInitFunction
	||
	(R|(R|e_InitFunctionName), (R|e_Foo), e_Text)	^ (R|"\t", e_InitFunctionName, "_", e_Foo, R<file18_MakeInitFunction, (R|e_InitFunctionName), e_Text>R)
	||
	(R|(R|e_InitFunctionName))	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"MakeInitFunction\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_GenerateFunCont() {
	file18_GenerateFunCont
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|e_Ident), e_FunCont)	^ (R|"\t", R<file18_NeedsPrefix, (R|e_Ident), (R|e_PublicFunctions), (R|e_ExternalFunctions), s_FileNumber>R, R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Ident)>R, R<file18_FunctionBody, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_FunCont>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"GenerateFunCont\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_FunctionBody() {
	file18_FunctionBody
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Foo, (R|s_LineNumber, s_DelimType, Semicolon), e_Text)	^ (R|"\n\t||\n\t", R<file18_Clause, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Foo>R, R<file18_FunctionBody, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Text>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber)	^ (~R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Foo)	^ (R|"\n\t||\n\t", R<file18_Clause, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Foo>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"FunctionBody\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_Clause() {
	file18_Clause
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Left, (R|s_LineNumber1, s_DelimType, Comma), e_Where, (R|s_LineNumber2, s_DelimType, Equal), e_Right)	^ (R|R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Left>R, " & ", R<file18_Where, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Where>R, "\t^ ", R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Right>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Left, (R|s_LineNumber, s_DelimType, Equal), e_Right)	^ (R|R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Left>R, "\t^ ", R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Right>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Left, (R|s_LineNumber1, s_DelimType, Comma), e_Where, (R|s_LineNumber2, s_DelimType, Colon), (R|(R|s_LineNumber3, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber4, s_DelimType, s_RCBracket)))	^ (R|R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Left>R, " & ", R<file18_Where, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Where, (R|s_LineNumber2, s_DelimType, Colon), (R|(R|s_LineNumber3, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber4, s_DelimType, s_RCBracket))>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Clause\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_Where() {
	file18_Where
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Right, (R|s_LineNumber1, s_DelimType, Colon), e_Left, (R|s_LineNumber, s_DelimType, Comma), e_Where)	^ (R|R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Right>R, " | ", R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Left>R, " & ", R<file18_Where, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Where>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Right, (R|s_LineNumber1, s_DelimType, Colon), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)))	^ (R|R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Right>R, " |  RWITH(\n", "\tRWITH", R<file18_FunctionBody, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_FunCont>R, "\n", "\t)")
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Right, (R|s_LineNumber, s_DelimType, Colon), e_Left)	^ (R|R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Right>R, " | ", R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Left>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Where\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_Expression() {
	file18_Expression
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, t_Term, e_Foo)	^ (R|"(R|", R<file18_SimpleExpression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, t_Term, e_Foo>R, ")")
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber)	^ (R|"(~R)")
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Expression\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_SimpleExpression() {
	file18_SimpleExpression
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, t_Term)	^ (R|R<file18_Term, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, t_Term>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, t_Term, e_Foo)	^ (R|R<file18_Term, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, t_Term>R, ", ", R<file18_SimpleExpression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Foo>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"SimpleExpression\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_Term() {
	file18_Term
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|(R|s_LineNumber1, s_DelimType, s_LBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RBracket))) & (R|R<GetDelimCode, "(">R) | (R|s_LBracket) & (R|R<GetDelimCode, ")">R) | (R|s_RBracket)	^ (R|R<file18_Expression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Foo>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|(R|s_LineNumber1, s_DelimType, s_LFBracket), (R|s_LineNumber2, s_SymbolType, e_Ident), (R|s_LineNumber3, s_DelimType, s_RFBracket))) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "<">R) | (R|s_LFBracket) & (R|R<GetDelimCode, ">">R) | (R|s_RFBracket)	^ (R|"R<", R<file18_NeedsPrefix, (R|e_Ident), (R|e_PublicFunctions), (R|e_ExternalFunctions), s_FileNumber>R, R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Ident)>R, ">R")
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|(R|s_LineNumber1, s_DelimType, s_LFBracket), (R|s_LineNumber2, s_SymbolType, e_Ident), e_Foo, (R|s_LineNumber3, s_DelimType, s_RFBracket))) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "<">R) | (R|s_LFBracket) & (R|R<GetDelimCode, ">">R) | (R|s_RFBracket)	^ (R|"R<", R<file18_NeedsPrefix, (R|e_Ident), (R|e_PublicFunctions), (R|e_ExternalFunctions), s_FileNumber>R, R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Ident)>R, ", ", R<file18_SimpleExpression, (R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, e_Foo>R, ">R")
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|s_LineNumber, s_VarType, e_Var)) & (R|R<GetVarType>R) | (R|s_VarType)	^ (R|R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Var)>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|s_LineNumber, s_SymbolType, e_Ident)) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|R<GenerateName, (R|e_NamesConvention), (R|e_CharsConvention), (R|e_Ident)>R)
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|s_LineNumber, s_TextConstantType, e_TextConstant)) & (R|R<GetTextConstantType>R) | (R|s_TextConstantType)	^ (R|"\"", e_TextConstant, "\"")
	||
	(R|(R|e_PublicFunctions), (R|e_ExternalFunctions), (R|e_NamesConvention), (R|e_CharsConvention), s_FileNumber, (R|s_LineNumber, s_NumberType, e_Number)) & (R|R<GetNumberType>R) | (R|s_NumberType)	^ (R|e_Number)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"Term\" function. View field: ", e_Foo>R);
}

void RefalInit_file18_GenerateInitFunctionArgc() {
	file18_GenerateInitFunctionArgc
	||
	(R|e_InitFunction)	^ (R|"\n", "extern void initarg(int argc, char **argv);\n", "\n", "void ", e_InitFunction, "(int argc, char **argv) {\n", "\t", e_InitFunction, "();\n", "\tinitarg(argc, argv);\n", "}\n");
}

void RefalInit_file18_GenerateRefalFree() {
	file18_GenerateRefalFree
	||
	(~R)	^ (R|"\n", "void RefalFree() {\n", "\t(R|R<Clear>R).Evaluate();\n", "}\n");
}

void RefalInit_GetCurrentVersion() {
	GetCurrentVersion
	||
	(~R)	^ (R|"0.3.0");
}

void RefalInit_VeryStart() {
	VeryStart
	||
	(R|e_Foo)	^ (R|R<Start, R<file20_ReadFiles, e_Foo>R>R);
}

void RefalInit_file20_ReadFiles() {
	file20_ReadFiles
	||
	(R|(R|e_FileName), e_Foo)	^ (R|(R|(R|e_FileName), R<file20_DeleteEndZero, R<file20_ReadFile, (R|e_FileName)>R>R), R<file20_ReadFiles, e_Foo>R)
	||
	(~R)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"ReadFiles\" function. View field: ", e_Foo>R);
}

void RefalInit_file20_ReadFile() {
	file20_ReadFile
	||
	(R|(R|e_FileName))	^ (R|R<Open, "r", 1, e_FileName>R, R<file20_ReadFileContents, R<Get, 1>R>R, R<Close, 1>R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"ReadFile\" function. View field: ", e_Foo>R);
}

void RefalInit_file20_ReadFileContents() {
	file20_ReadFileContents
	||
	(R|0)	^ (~R)
	||
	(R|e_Foo)	^ (R|e_Foo, "\n", R<file20_ReadFileContents, R<Get, 1>R>R);
}

void RefalInit_file20_DeleteEndZero() {
	file20_DeleteEndZero
	||
	(R|e_Foo, 0, e_Bar)	^ (R|e_Foo)
	||
	(R|e_Foo)	^ (R|e_Foo);
}

void RefalInit_RemoveSC() {
	RemoveSC
	||
	(R|(R|s_LineNumber0, s_ExternType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|s_LineNumber2, s_DelimType, Semicolon), e_Text) & (R|R<GetExternType>R) | (R|s_ExternType) & (R|R<GetSymbolType>R) | (R|s_SymbolType)	^ (R|(R|s_LineNumber0, s_ExternType), (R|s_LineNumber1, s_SymbolType, e_Symbol), (R|s_LineNumber2, s_DelimType, Semicolon), R<RemoveSC, e_Text>R)
	||
	(R|(R|s_LineNumber0, s_DirectivesType), (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), e_SemiOrEmpty, e_Text) & (R|R<GetDirectType>R) | (R|s_DirectivesType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket) & (R|R<file21_IsSemiOrEmpty, e_SemiOrEmpty>R) | (R|True)	^ (R|(R|s_LineNumber0, s_DirectivesType), (R|(R|s_LineNumber1, s_DelimType, s_LCBracket), e_Foo, (R|s_LineNumber2, s_DelimType, s_RCBracket)), R<RemoveSC, e_Text>R)
	||
	(R|(R|s_LineNumber0, s_EntryType), (R|s_LineNumber1, s_SymbolType, e_Ident), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_SemiOrEmpty, e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetEntryType>R) | (R|s_EntryType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket) & (R|R<file21_IsSemiOrEmpty, e_SemiOrEmpty>R) | (R|True)	^ (R|(R|s_LineNumber0, s_EntryType), (R|s_LineNumber1, s_SymbolType, e_Ident), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), R<RemoveSC, e_Text>R)
	||
	(R|(R|s_LineNumber1, s_SymbolType, e_Ident), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), e_SemiOrEmpty, e_Text) & (R|R<GetSymbolType>R) | (R|s_SymbolType) & (R|R<GetDelimCode, "{">R) | (R|s_LCBracket) & (R|R<GetDelimCode, "}">R) | (R|s_RCBracket) & (R|R<file21_IsSemiOrEmpty, e_SemiOrEmpty>R) | (R|True)	^ (R|(R|s_LineNumber1, s_SymbolType, e_Ident), (R|(R|s_LineNumber2, s_DelimType, s_LCBracket), e_FunCont, (R|s_LineNumber3, s_DelimType, s_RCBracket)), R<RemoveSC, e_Text>R)
	||
	(R|t_Term, e_Foo)	^ (R|t_Term, R<RemoveSC, e_Foo>R)
	||
	(~R)	^ (~R)
	||
	(R|e_Foo)	^ (R|R<Prout, "Bug in \"RemoveSC\" function. View field: ", e_Foo>R);
}

void RefalInit_file21_IsSemiOrEmpty() {
	file21_IsSemiOrEmpty
	||
	(R|(R|s_LineNumber, s_DelimType, Semicolon))	^ (R|True)
	||
	(R|e_Foo)	^ (R|False);
}


void RefalInit() {
	RefalInit_Start();
	RefalInit_file2_Process();
	RefalInit_Lexical();
	RefalInit_file3_FirstLineNumber();
	RefalInit_file3_StateS();
	RefalInit_file3_StatePragma();
	RefalInit_file3_StatePrefixEmptyPragma();
	RefalInit_file3_StateSymbolPragma();
	RefalInit_file3_StateTextConstantPragma();
	RefalInit_file3_StateMultiComment();
	RefalInit_file3_StateComment();
	RefalInit_file3_StateTextConstant();
	RefalInit_file3_StateSymbol();
	RefalInit_file3_StatePrefixEmpty();
	RefalInit_file3_StateInteger();
	RefalInit_file3_StateIntegerEmpty();
	RefalInit_file3_StateSuffix();
	RefalInit_file3_StateFract();
	RefalInit_file3_StateNumberSuffixEmpty();
	RefalInit_file3_StateIdentSuffixEmpty();
	RefalInit_file3_StateFractEmpty();
	RefalInit_file3_IsSpaceOrTab();
	RefalInit_file3_IsDelimiter();
	RefalInit_IsLetter();
	RefalInit_IsDigit();
	RefalInit_AddError();
	RefalInit_PrintErrors();
	RefalInit_file4_NoErrors();
	RefalInit_file4_FirstErrorNumber();
	RefalInit_file4_CountErrors();
	RefalInit_file4_PrintHeader();
	RefalInit_file4_CorrectPhrase1();
	RefalInit_file4_CorrectPhrase2();
	RefalInit_file4_PrintList();
	RefalInit_file4_Description();
	RefalInit_file4_CorrectPhrase3();
	RefalInit_file4_NumbersList();
	RefalInit_file4_DescriptionBody();
	RefalInit_file4_LookAtDesc();
	RefalInit_FirstLine();
	RefalInit_LastLine();
	RefalInit_GetErrorsList();
	RefalInit_CheckLexical();
	RefalInit_Syntax();
	RefalInit_file6_SyntaxMain();
	RefalInit_file6_GatherExtern();
	RefalInit_file6_AddExtern();
	RefalInit_file6_StartSyntax();
	RefalInit_file6_SyntaxFunCont();
	RefalInit_file6_StartFun();
	RefalInit_file6_SyntaxOneClause();
	RefalInit_file6_SyntaxWherePart();
	RefalInit_file6_AddVar();
	RefalInit_file6_IsStandard();
	RefalInit_file6_SyntaxLeftPart();
	RefalInit_file6_SyntaxRightPart();
	RefalInit_file6_GetFirstTerm();
	RefalInit_file6_IsSymbolOrConstant();
	RefalInit_file6_CheckCalledFun();
	RefalInit_file6_CheckVar();
	RefalInit_Brackets();
	RefalInit_file7_PairBrackets();
	RefalInit_file7_HasThis();
	RefalInit_file7_HasLeftBrackets();
	RefalInit_file7_FindPair();
	RefalInit_file7_IsLeftBracket();
	RefalInit_file7_IsRightBracket();
	RefalInit_GetEntryType();
	RefalInit_GetExternType();
	RefalInit_GetDirectType();
	RefalInit_GetDelimType();
	RefalInit_GetTextConstantType();
	RefalInit_GetSymbolType();
	RefalInit_GetNumberType();
	RefalInit_GetVarType();
	RefalInit_GetDelimCode();
	RefalInit_file8_GetDecode();
	RefalInit_Reverse2();
	RefalInit_Reverse();
	RefalInit_Decode();
	RefalInit_GetLibraryFuns();
	RefalInit_Library();
	RefalInit_file9_AddZero();
	RefalInit_Unimplemented();
	RefalInit_file9_NotInStandard();
	RefalInit_file9_IsUnimplemented();
	RefalInit_file9_CheckBuiltin();
	RefalInit_file9_ProcessBuiltin();
	RefalInit_CheckSyntax();
	RefalInit_SyntaxDirectives();
	RefalInit_file11_SyntaxOne();
	RefalInit_Linker();
	RefalInit_file12_DirectivesTypesNumber();
	RefalInit_file12_ListsNumber();
	RefalInit_NextNumbersFrom();
	RefalInit_file12_GetDirectives();
	RefalInit_file12_MergeInFiles();
	RefalInit_file12_Merge();
	RefalInit_file12_GetSomething();
	RefalInit_file12_GetSomethingMore();
	RefalInit_file12_GatherRest();
	RefalInit_file12_Gather();
	RefalInit_file12_GatherMore();
	RefalInit_file12_GatherInFiles();
	RefalInit_file12_GatherInOneFile();
	RefalInit_file12_GatherVars();
	RefalInit_file12_GatherSymbols();
	RefalInit_file12_GatherEntries();
	RefalInit_file12_GatherExterns();
	RefalInit_file12_AddVarOrSymbol();
	RefalInit_file12_AddEntry();
	RefalInit_MoreLinker();
	RefalInit_file13_MakeCheck();
	RefalInit_file13_ChecksNumber();
	RefalInit_NotEmptyPublics();
	RefalInit_file13_MoreChecks();
	RefalInit_file13_NoDupesInEntries();
	RefalInit_file13_NoUnresolvedExternals();
	RefalInit_file13_EveryPublicIsEntry();
	RefalInit_CheckLinker();
	RefalInit_Generate();
	RefalInit_GenerateName();
	RefalInit_AddSymbol();
	RefalInit_TwoNewlines();
	RefalInit_IsStatic();
	RefalInit_GenerateHxx();
	RefalInit_file16_GetHxxName();
	RefalInit_file16_HxxPartsNumber();
	RefalInit_file16_GetHxxCont();
	RefalInit_file16_FillHxx();
	RefalInit_file16_StartFilling();
	RefalInit_file16_AddHeader();
	RefalInit_file16_AddInitFunction();
	RefalInit_GenerateCxx();
	RefalInit_file17_GetCxxName();
	RefalInit_file17_CxxPartsNumber();
	RefalInit_file17_GetCxxCont();
	RefalInit_file17_FillCxx();
	RefalInit_file17_StartFilling();
	RefalInit_file17_AddHeader();
	RefalInit_file17_AddUsedModules();
	RefalInit_file17_AddLibrary();
	RefalInit_file17_AddAllSymbols();
	RefalInit_file17_AddAllVariables();
	RefalInit_file17_GetPrefix();
	RefalInit_file17_ToUpper();
	RefalInit_file18_FirstFileNumber();
	RefalInit_AddFunctionsCode();
	RefalInit_file18_StartAdding();
	RefalInit_file18_AddingFromOneFile();
	RefalInit_file18_NeedsPrefix();
	RefalInit_file18_AddToList();
	RefalInit_file18_FinishAdding();
	RefalInit_file18_AddInitFunction();
	RefalInit_file18_MakeInitFunction();
	RefalInit_file18_GenerateFunCont();
	RefalInit_file18_FunctionBody();
	RefalInit_file18_Clause();
	RefalInit_file18_Where();
	RefalInit_file18_Expression();
	RefalInit_file18_SimpleExpression();
	RefalInit_file18_Term();
	RefalInit_file18_GenerateInitFunctionArgc();
	RefalInit_file18_GenerateRefalFree();
	RefalInit_GetCurrentVersion();
	RefalInit_VeryStart();
	RefalInit_file20_ReadFiles();
	RefalInit_file20_ReadFile();
	RefalInit_file20_ReadFileContents();
	RefalInit_file20_DeleteEndZero();
	RefalInit_RemoveSC();
	RefalInit_file21_IsSemiOrEmpty();
}

extern void initarg(int argc, char **argv);

void RefalInit(int argc, char **argv) {
	RefalInit();
	initarg(argc, argv);
}

void RefalFree() {
	(R|R<Clear>R).Evaluate();
}

