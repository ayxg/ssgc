#pragma once
#include "cnd_constdef.hpp"

///////////////////////////////////////////////////////////////////////////////
/// @section Local Macros
/// To ease generation of methods for large enums, eg. enum to string methods.
/// !warning: Undefined at the end of the file.
///////////////////////////////////////////////////////////////////////////////

#define CND_MM_EXPAND(x) x
#define CND_MM_COMMA ,
#define CND_MM_IDTOSTR(x) #x
#define CND_MM_CONST_ID_TAG(x) k##x

/// Creates an enum from an applied list. Adds a COUNT entry at the end.
/// @pre 'COUNT' must NOT be an identifier in the applied list.
/// @param ln Applied list macro name.
/// @param en Enum name. Fully qualified or call macro inside the namespace.
/// @param et Enum type. Underlying type of the enum. Must be provided.
/// 
/// !A 'COUNT' entry is always added after the last entry, at the end.
/// !Do not add the conventional 'k' prefix to the enum entry names. The 'k' prefix is added by the macro.
#define CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM(ln, en, et) \
  enum class en : et { CND_APPLIED_ENUM_##ln(CND_MM_CONST_ID_TAG, CND_MM_COMMA, , CND_MM_COMMA COUNT) }
#define CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(ln, en) \
  enum class en { CND_APPLIED_ENUM_##ln(CND_MM_CONST_ID_TAG, CND_MM_COMMA, , CND_MM_COMMA COUNT) }

/// Creates a switch case for a macro identifier to string conversion.
#define CND_MM_IDENT_TO_CSTR_SWITCH_CASE(x) \
  case CND_MM_CONST_ID_TAG(x):              \
    return CND_MM_IDTOSTR(k##x);

/// Creates a method to convert an enum to a string given an applied list.
/// @pre Enum must be defined using CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM
/// @param fn : Function name to be defined.
/// @param ln : Applied list name, make sure to include namespace in the name.
/// @param en : Enum name with full namespace from the global ns, or call macro
///             inside the ns.
#define CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(fn, ln, en)                  \
  CND_CX const char* fn(en e) {                                              \
    using enum en;                                                           \
    switch (e) {                                                             \
      CND_APPLIED_ENUM_##ln(CND_MM_IDENT_TO_CSTR_SWITCH_CASE, , , case COUNT \
                            : return "COUNT";) default : return "<invalid>"; \
    }                                                                        \
  }

#define CND_APPLIED_ENUM_ENTRY(n) sep m(n)

///////////////////////////////////////////////////////////////////////////////
/// @end-section Local Macros
///////////////////////////////////////////////////////////////////////////////

namespace cnd {
///////////////////////////////////////////////////////////////////////////////
/// @section Compiler enum applied list definitions.
/// Add new enumerations to the large macro defs here.
/// Current "large" enums:
///   - cnd::fw::eOpSys
///   - cnd::fw::eProcArch
///   - cnd::fw::eToolchain
///   - cnd::grammar::eSrcChar
///   - cnd::grammar::eTk
///   - cnd::grammar::eCst
///   - cnd::clerr::eClErr
///
/// An applied list will have an optional prefix and apply a macro to each
/// element in the list followed by a separator. The last element will have no
/// separator, ends is 'lst'.
///
/// Since there is currently no reflection support, this seems like the most
/// sane method to associate identifiers with enums and convert them to strings,
/// or other identifiers.
///
/// Limits:
///   - The list cannot have defined values, only identifiers.
///   - The enums are all contiguous, starting from 0.
///   - The enums all end in a COUNT enumeration entry.
///////////////////////////////////////////////////////////////////////////////

/// define enum cnd::eOpSys
#define CND_APPLIED_ENUM_eOpSys(m, sep, pre, lst) \
  pre m(INVALID)                                  \
  sep m(NONE)                                     \
  sep m(Custom)                                   \
  sep m(Embedded)                                 \
  sep m(AIX)                                      \
  sep m(Android)                                  \
  sep m(AmdahlUTS)                                \
  sep m(AmigaOS)                                  \
  sep m(ApolloAEGIS)                              \
  sep m(ApolloDomainOS)                           \
  sep m(Bada)                                     \
  sep m(BeOS)                                     \
  sep m(BlueGene)                                 \
  sep m(BSDOpen)                                  \
  sep m(BSDNet)                                   \
  sep m(BSD)                                      \
  sep m(ConvexOS)                                 \
  sep m(CygwinEnvironment)                        \
  sep m(DGUX)                                     \
  sep m(DragonFly)                                \
  sep m(DYNIXPtx)                                 \
  sep m(ECos)                                     \
  sep m(EMXEnvironment)                           \
  sep m(FreeBSD)                                  \
  sep m(GNUHurd)                                  \
  sep m(GNUkFreeBSD)                              \
  sep m(GNULinux)                                 \
  sep m(HIUXMPP)                                  \
  sep m(HPUX)                                     \
  sep m(IBMOS400)                                 \
  sep m(INTEGRITY)                                \
  sep m(InterixEnvironment)                       \
  sep m(IRIX)                                     \
  sep m(IOS)                                      \
  sep m(Linux)                                    \
  sep m(LynxOS)                                   \
  sep m(MacOS)                                    \
  sep m(MicrowareOS9)                             \
  sep m(MINIX)                                    \
  sep m(MorphOS)                                  \
  sep m(MPEiX)                                    \
  sep m(MSDOS)                                    \
  sep m(NetBSD)                                   \
  sep m(NonStop)                                  \
  sep m(NucleusRTOS)                              \
  sep m(OpenBSD)                                  \
  sep m(OS2)                                      \
  sep m(PalmOS)                                   \
  sep m(Plan9)                                    \
  sep m(PyramidDCOSx)                             \
  sep m(QNX)                                      \
  sep m(ReliantUNIX)                              \
  sep m(SCOOpenServer)                            \
  sep m(Solaris)                                  \
  sep m(StratusVOS)                               \
  sep m(SVR4Environment)                          \
  sep m(Syllable)                                 \
  sep m(SymbianOS)                                \
  sep m(Tru64OSF1)                                \
  sep m(Ultrix)                                   \
  sep m(UNICOS)                                   \
  sep m(UNICOSmp)                                 \
  sep m(UNIXEnvironment)                          \
  sep m(UnixWare)                                 \
  sep m(UWinEnvironment)                          \
  sep m(VMS)                                      \
  sep m(VxWorks)                                  \
  sep m(Windows)                                  \
  sep m(WindowsCE)                                \
  sep m(WindUEnvironment)                         \
  sep m(zOS)                                      \
  lst
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eOpSys, eOpSys);

/// define enum cnd::eProcArch
#define CND_APPLIED_ENUM_eProcArch(m, sep, pre, lst) \
  pre m(INVALID)                                     \
  sep m(NONE)                                        \
  sep m(Custom)                                      \
  sep m(Embedded)                                    \
  sep m(Alpha)                                       \
  sep m(AMD64)                                       \
  sep m(ARM)                                         \
  sep m(ARM64)                                       \
  sep m(Blackfin)                                    \
  sep m(Convex)                                      \
  sep m(Epiphany)                                    \
  sep m(HppaRISC)                                    \
  sep m(Intel86)                                     \
  sep m(IntelItanium)                                \
  sep m(Motorola68k)                                 \
  sep m(MIPS)                                        \
  sep m(PowerPC)                                     \
  sep m(Pyramid9810)                                 \
  sep m(RS6000)                                      \
  sep m(SPARC)                                       \
  sep m(SuperH)                                      \
  sep m(SystemZ)                                     \
  sep m(TMS320)                                      \
  sep m(TMS470)                                      \
  lst
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eProcArch, eProcArch);

/// define enum cnd::eToolchain
#define CND_APPLIED_ENUM_eToolchain(m, sep, pre, lst) \
  pre m(INVALID)                                      \
  sep m(NONE)                                         \
  sep m(Custom)                                       \
  sep m(Embedded)                                     \
  sep m(ACC)                                          \
  sep m(AltiumMicroBlazeC)                            \
  sep m(AltiumCToHardware)                            \
  sep m(AmsterdamCompilerKit)                         \
  sep m(ARM)                                          \
  sep m(AztecC)                                       \
  sep m(Bordland)                                     \
  sep m(CC65)                                         \
  sep m(Clang)                                        \
  sep m(Comeau)                                       \
  sep m(Compaq)                                       \
  sep m(ConvexC)                                      \
  sep m(CompCert)                                     \
  sep m(CrayC)                                        \
  sep m(Diab)                                         \
  sep m(DiceC)                                        \
  sep m(DigitalMars)                                  \
  sep m(DignusSystems)                                \
  sep m(DJGPP)                                        \
  sep m(EDGFrontend)                                  \
  sep m(EKOPath)                                      \
  sep m(Fujitsu)                                      \
  sep m(GCC)                                          \
  sep m(GreenHill)                                    \
  sep m(HPAnsiC)                                      \
  sep m(HPa)                                          \
  sep m(IAR)                                          \
  sep m(IBMXL)                                        \
  sep m(IBMXLLegacy)                                  \
  sep m(IBMXLMainframe)                               \
  sep m(ImageCraftC)                                  \
  sep m(Intel)                                        \
  sep m(Kai)                                          \
  sep m(KEILCARM)                                     \
  sep m(KEILC166)                                     \
  sep m(KEILC51)                                      \
  sep m(LCC)                                          \
  sep m(LLVM)                                         \
  sep m(MetaWareHigh)                                 \
  sep m(MetrowerksCodeWarrior)                        \
  sep m(MSVC)                                         \
  sep m(Microtec)                                     \
  sep m(MicrowayNDPC)                                 \
  sep m(MinGW)                                        \
  sep m(MinGW64)                                      \
  sep m(MIPSpro)                                      \
  sep m(MiracleC)                                     \
  sep m(MPW)                                          \
  sep m(NorcroftC)                                    \
  sep m(NWCC)                                         \
  sep m(Open64)                                       \
  sep m(OracleProCPrecompiler)                        \
  sep m(OracleSolarisStudio)                          \
  sep m(PacificC)                                     \
  sep m(Palm)                                         \
  sep m(PellesC)                                      \
  sep m(PortlandGroup)                                \
  sep m(Renesas)                                      \
  sep m(SASC)                                         \
  sep m(SCOOpenServer)                                \
  sep m(SmallDeviceC)                                 \
  sep m(SN)                                           \
  sep m(StratusVOSC)                                  \
  sep m(SymantecC)                                    \
  sep m(TenDRA)                                       \
  sep m(TexasInstruments)                             \
  sep m(THINKC)                                       \
  sep m(TinyC)                                        \
  sep m(Turbo)                                        \
  sep m(Ultimate)                                     \
  sep m(USLC)                                         \
  sep m(VBCC)                                         \
  sep m(WatcomC)                                      \
  sep m(ZortechC)                                     \
  lst
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eToolchain, eToolchain);

/// define enum cnd::eClCliParam
#define CND_APPLIED_ENUM_eClCliParam(m, sep, pre, lst) \
  pre m(INVALID)                                       \
  sep m(NONE)                                          \
  sep m(Help)                                          \
  sep m(OutDir)                                        \
  sep m(AuxDir)                                        \
  sep m(PredefSymbol)                                  \
  lst
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClCliParam, eClCliParam);

/// define enum cnd::eSrcChar
#define CND_APPLIED_ENUM_eSrcChar(m, sep, pre, lst) \
  pre m(Null)                                       \
  sep m(StartOfHeader)                              \
  sep m(StartOfText)                                \
  sep m(EndOfText)                                  \
  sep m(EndOfTransmission)                          \
  sep m(Enquiry)                                    \
  sep m(Acknowledge)                                \
  sep m(Bell)                                       \
  sep m(Backspace)                                  \
  sep m(HorizontalTab)                              \
  sep m(LineFeed)                                   \
  sep m(VerticalTab)                                \
  sep m(FormFeed)                                   \
  sep m(CarriageReturn)                             \
  sep m(ShiftOut)                                   \
  sep m(ShiftIn)                                    \
  sep m(DataLinkEscape)                             \
  sep m(DeviceControl1)                             \
  sep m(DeviceControl2)                             \
  sep m(DeviceControl3)                             \
  sep m(DeviceControl4)                             \
  sep m(NegativeAcknowledge)                        \
  sep m(SynchronousIdle)                            \
  sep m(EndOfTransmissionBlock)                     \
  sep m(Cancel)                                     \
  sep m(EndOfMedium)                                \
  sep m(Substitute)                                 \
  sep m(Escape)                                     \
  sep m(FileSeparator)                              \
  sep m(GroupSeparator)                             \
  sep m(RecordSeparator)                            \
  sep m(UnitSeparator)                              \
  sep m(Space)                                      \
  sep m(ExclamationMark)                            \
  sep m(Quotation)                                  \
  sep m(Hash)                                       \
  sep m(DollarSign)                                 \
  sep m(PercentSign)                                \
  sep m(Ampersand)                                  \
  sep m(Apostrophe)                                 \
  sep m(LeftParenthesis)                            \
  sep m(RightParenthesis)                           \
  sep m(Asterisk)                                   \
  sep m(PlusSign)                                   \
  sep m(Comma)                                      \
  sep m(HyphenMinus)                                \
  sep m(Period)                                     \
  sep m(Solidus)                                    \
  sep m(Zero)                                       \
  sep m(One)                                        \
  sep m(Two)                                        \
  sep m(Three)                                      \
  sep m(Four)                                       \
  sep m(Five)                                       \
  sep m(Six)                                        \
  sep m(Seven)                                      \
  sep m(Eight)                                      \
  sep m(Nine)                                       \
  sep m(Colon)                                      \
  sep m(Semicolon)                                  \
  sep m(LessThan)                                   \
  sep m(EqualSign)                                  \
  sep m(GreaterThan)                                \
  sep m(QuestionMark)                               \
  sep m(AtSign)                                     \
  sep m(UpperA)                                     \
  sep m(UpperB)                                     \
  sep m(UpperC)                                     \
  sep m(UpperD)                                     \
  sep m(UpperE)                                     \
  sep m(UpperF)                                     \
  sep m(UpperG)                                     \
  sep m(UpperH)                                     \
  sep m(UpperI)                                     \
  sep m(UpperJ)                                     \
  sep m(UpperK)                                     \
  sep m(UpperL)                                     \
  sep m(UpperM)                                     \
  sep m(UpperN)                                     \
  sep m(UpperO)                                     \
  sep m(UpperP)                                     \
  sep m(UpperQ)                                     \
  sep m(UpperR)                                     \
  sep m(UpperS)                                     \
  sep m(UpperT)                                     \
  sep m(UpperU)                                     \
  sep m(UpperV)                                     \
  sep m(UpperW)                                     \
  sep m(UpperX)                                     \
  sep m(UpperY)                                     \
  sep m(UpperZ)                                     \
  sep m(LeftSquareBracket)                          \
  sep m(Backslash)                                  \
  sep m(RightSquareBracket)                         \
  sep m(Caret)                                      \
  sep m(Underscore)                                 \
  sep m(Backtick)                                   \
  sep m(LowerA)                                     \
  sep m(LowerB)                                     \
  sep m(LowerC)                                     \
  sep m(LowerD)                                     \
  sep m(LowerE)                                     \
  sep m(LowerF)                                     \
  sep m(LowerG)                                     \
  sep m(LowerH)                                     \
  sep m(LowerI)                                     \
  sep m(LowerJ)                                     \
  sep m(LowerK)                                     \
  sep m(LowerL)                                     \
  sep m(LowerM)                                     \
  sep m(LowerN)                                     \
  sep m(LowerO)                                     \
  sep m(LowerP)                                     \
  sep m(LowerQ)                                     \
  sep m(LowerR)                                     \
  sep m(LowerS)                                     \
  sep m(LowerT)                                     \
  sep m(LowerU)                                     \
  sep m(LowerV)                                     \
  sep m(LowerW)                                     \
  sep m(LowerX)                                     \
  sep m(LowerY)                                     \
  sep m(LowerZ)                                     \
  sep m(LeftCurlyBrace)                             \
  sep m(VerticalBar)                                \
  sep m(RightCurlyBrace)                            \
  sep m(Tilde)                                      \
  sep m(Delete)                                     \
  lst
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eSrcChar, eSrcChar);

/// define enum cnd::eTk
#define CND_APPLIED_ENUM_eTk(m, sep, pre, lst) \
  pre m(INVALID)                               \
  sep m(NONE)                                  \
  sep m(DirectiveInclude)                      \
  sep m(DirectiveDefMacro)                     \
  sep m(DirectiveEndmacro)                     \
  sep m(DirectiveIf)                           \
  sep m(DirectiveElse)                         \
  sep m(DirectiveElif)                         \
  sep m(DirectiveEndif)                        \
  sep m(DirectiveIfdef)                        \
  sep m(DirectiveIfndef)                       \
  sep m(DirectiveUndef)                        \
  sep m(KwDef)                                 \
  sep m(KwFn)                                  \
  sep m(KwClass)                               \
  sep m(KwMain)                                \
  sep m(KwImport)                              \
  sep m(KwNamespace)                           \
  sep m(KwUse)                                 \
  sep m(KwLib)                                 \
  sep m(KwDll)                                 \
  sep m(KwEnum)                                \
  sep m(KwIf)                                  \
  sep m(KwElif)                                \
  sep m(KwElse)                                \
  sep m(KwCxif)                                \
  sep m(KwCxelif)                              \
  sep m(KwCxelse)                              \
  sep m(KwSwitch)                              \
  sep m(KwCase)                                \
  sep m(KwDefault)                             \
  sep m(KwWhile)                               \
  sep m(KwFor)                                 \
  sep m(KwReturn)                              \
  sep m(KwBreak)                               \
  sep m(KwContinue)                            \
  sep m(KwInt)                                 \
  sep m(KwUint)                                \
  sep m(KwReal)                                \
  sep m(KwBool)                                \
  sep m(KwChar)                                \
  sep m(KwByte)                                \
  sep m(KwCstr)                                \
  sep m(KwStr)                                 \
  sep m(KwPtr)                                 \
  sep m(KwList)                                \
  sep m(KwArray)                               \
  sep m(KwTrue)                                \
  sep m(KwFalse)                               \
  sep m(KwNone)                                \
  sep m(KwVoid)                                \
  sep m(KwIn)                                  \
  sep m(KwAs)                                  \
  sep m(KwCin)                                 \
  sep m(KwCout)                                \
  sep m(KwNative)                              \
  sep m(KwConst)                               \
  sep m(KwRef)                                 \
  sep m(KwPrivate)                             \
  sep m(KwPublic)                              \
  sep m(KwStatic)                              \
  sep m(KwAny)                                 \
  sep m(KwAuto)                                \
  sep m(KwType)                                \
  sep m(KwValue)                               \
  sep m(KwTemplate)                            \
  sep m(Hash)                                  \
  sep m(Add)                                   \
  sep m(Sub)                                   \
  sep m(Mul)                                   \
  sep m(Div)                                   \
  sep m(Mod)                                   \
  sep m(And)                                   \
  sep m(Or)                                    \
  sep m(Xor)                                   \
  sep m(Not)                                   \
  sep m(Lsh)                                   \
  sep m(Rsh)                                   \
  sep m(Eq)                                    \
  sep m(Neq)                                   \
  sep m(Lt)                                    \
  sep m(Gt)                                    \
  sep m(Lte)                                   \
  sep m(Gte)                                   \
  sep m(Spaceship)                             \
  sep m(Assign)                                \
  sep m(NewAssign)                             \
  sep m(AddAssign)                             \
  sep m(SubAssign)                             \
  sep m(MulAssign)                             \
  sep m(DivAssign)                             \
  sep m(ModAssign)                             \
  sep m(AndAssign)                             \
  sep m(OrAssign)                              \
  sep m(XorAssign)                             \
  sep m(LshAssign)                             \
  sep m(RshAssign)                             \
  sep m(Inc)                                   \
  sep m(Dec)                                   \
  sep m(Dot)                                   \
  sep m(Bnot)                                  \
  sep m(Band)                                  \
  sep m(Bor)                                   \
  sep m(LParen)                                \
  sep m(RParen)                                \
  sep m(LBrace)                                \
  sep m(RBrace)                                \
  sep m(LBracket)                              \
  sep m(RBracket)                              \
  sep m(Semicolon)                             \
  sep m(Colon)                                 \
  sep m(Comma)                                 \
  sep m(Period)                                \
  sep m(DoubleColon)                           \
  sep m(Ellipsis)                              \
  sep m(CommercialAt)                          \
  sep m(LitInt)                                \
  sep m(LitUint)                               \
  sep m(LitReal)                               \
  sep m(LitBool)                               \
  sep m(LitChar)                               \
  sep m(LitByte)                               \
  sep m(LitCstr)                               \
  sep m(Ident)                                 \
  sep m(Eofile)                                \
  sep m(Whitespace)                            \
  sep m(Newline)                               \
  sep m(BlockComment)                          \
  sep m(LineComment)                           \
  sep m(Dollar)                                \
  sep m(Backlash)                              \
  sep m(Question)                              \
  lst
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eTk, eTk);

/// define enum cnd::eCst
/// @note all of eTk is part of eCst appended at the front.
#define CND_APPLIED_ENUM_eCst(m, sep, pre, lst)              \
  CND_APPLIED_ENUM_eTk(m, sep, pre, ) sep m(UndefinedObject) \
  sep m(Statement)                                           \
  sep m(Expression)                                          \
  sep m(Declaration)                                         \
  sep m(TypeAlias)                                           \
  sep m(LibraryTypeAlias)                                    \
  sep m(LibraryObjectInclusion)                              \
  sep m(LibraryNamespaceInclusion)                           \
  sep m(NamespaceInclusion)                                  \
  sep m(NamespaceObjectInclusion)                            \
  sep m(VariableDeclaration)                                 \
  sep m(VariableDefinition)                                  \
  sep m(MethodDeclaration)                                   \
  sep m(MethodDefinition)                                    \
  sep m(ClassDefinition)                                     \
  sep m(ClassDeclaration)                                    \
  sep m(ImportDeclaration)                                   \
  sep m(LibraryDeclaration)                                  \
  sep m(LibraryDefinition)                                   \
  sep m(MainDeclaration)                                     \
  sep m(MainDefinition)                                      \
  sep m(MethodVoid)                                          \
  sep m(MethodSignature)                                     \
  sep m(MethodParameter)                                     \
  sep m(MethodParameterList)                                 \
  sep m(MethodReturnType)                                    \
  sep m(IfStatement)                                         \
  sep m(Program)                                             \
  sep m(PragmaticBlock)                                      \
  sep m(FunctionalBlock)                                     \
  sep m(ConditionalBlock)                                    \
  sep m(IterativeBlock)                                      \
  sep m(TypeConstraints)                                     \
  sep m(CaptureList)                                         \
  sep m(Subexpression)                                       \
  sep m(TypeList)                                            \
  sep m(GenericList)                                         \
  sep m(FunctionCall)                                        \
  sep m(Arguments)                                           \
  sep m(TypeCall)                                            \
  sep m(TypeArguments)                                       \
  sep m(IndexOperator)                                       \
  sep m(ListingOperator)                                     \
  sep m(IndexArguments)                                      \
  sep m(Modifiers)                                           \
  sep m(UnaryMinus)                                          \
  sep m(Dereference)                                         \
  sep m(AddressOf)                                           \
  sep m(EnumDeclaration)                                     \
  sep m(EnumDefinition)                                      \
  sep m(EnumAssociation)                                     \
  sep m(EnumBlock)                                           \
  sep m(EnumCategory)                                        \
  sep m(EnumEntry)                                           \
  lst
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eCst, eCst);

/// define enum cnd::eClErr
#define CND_APPLIED_ENUM_eClErr(m, sep, pre, lst)       \
  pre m(CustomError)                                    \
  sep m(NoError)                                        \
  sep m(NotImplemented)                                 \
  sep m(FailedToReadFile)                               \
  sep m(UnknownSrcChar)                                 \
  sep m(InvalidCliArg)                                  \
  sep m(CliFlagMustHavePostfix)                         \
  sep m(CliFlagExpectedArgs)                            \
  sep m(CliFlagInvalidArg)                              \
  sep m(CliDeniedOverwrite)                             \
  sep m(CliFailedToRedirectStream)                      \
  sep m(ParserExpectedOpeningScope)                     \
  sep m(ParserOpeningScopeAtEof)              \
  sep m(ParserClosingScopeBeforeOpen) \
  sep m(ParserMismatchedParenScope)                          \
  sep m(ParserMismatchedBracketScope)                  \
  sep m(ParserMismatchedBraceScope)                          \
  lst
//sep m(ImplExpectedToken)                        \
  //sep m(InvalidCliArg)                            \
  //sep m(InclusionFailure)                         \
  //sep m(ForbiddenSourceChar)                      \
  //sep m(LexerUnknownChar)                         \
  //sep m(MismatchedScope)                          \
  //sep m(ExpectedPragmaticDeclaration)             \
  //sep m(ExpectedPrimaryExpression)                \
  //sep m(NotImplemented)                           \
  //sep m(InvalidForLoopSyntax)                     \
  //sep m(UserSyntaxError)                          \
  //sep m(InvalidSingularOperand)                   \
  //sep m(ParserExpectedToken)                      \
  //sep m(CevalIntegerOverflow)                     \
  //sep m(CevalUnsignedOverflow)                    \
  //sep m(CevalRealOverflow)                        \
  //sep m(CevalInvalidBoolLiteral)                  \
  //sep m(CevalInvalidCharLiteral)                  \
  //sep m(CevalInvalidByteLiteral)                  \
  //sep m(CevalInvalidStringLiteral)                \
  //sep m(InvalidEscapeSequence)

CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClErr, eClErr);

#define CND_APPLIED_ENUM_eClWarning(m, sep, pre, lst) \
  pre m(UndocumentedWarning)                          \
  lst
CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM_UNTYPED(eClWarning, eClWarning);

enum class eCharEncoding { kAscii, kUtf8, kUtf16, kUtf32, COUNT };

enum class eProgLang { kAsm, kCand, kC, kCpp, COUNT };

enum class eAssoc { INVALID, NONE, Left, Right, COUNT };

enum class eOperation { INVALID, NONE, Binary, Prefix, Postfix, COUNT };

enum class ePriority {
  INVALID = 0,
  NONE = 1,
  Assignment = 20'000,
  LogicalOr = 60'000,
  LogicalAnd = 70'000,
  BitwiseOr = 80'000,
  BitwiseXor = 85'000,
  BitwiseAnd = 90'000,
  Equality = 100'000,
  ThreeWayEquality = 110'000,
  Comparison = 120'000,
  Bitshift = 130'000,
  Term = 140'000,
  Factor = 150'000,
  Prefix = 160'000,
  Postfix = 170'000,
  Functional = 180'000,
  Access = 190'000,
  Max = INT_MAX,
};

enum class eClErrType { kSingle, kChain, COUNT };

enum class eClErrCat {
  kNone,
  kWarning,
  kCompilerImpl,
  kCommandLine,
  kGeneral,
  kScanner,
  kLexer,
  kParser,
  kPreprocessor,
  kConstEval,
  COUNT
};

enum class eVerbosityLevel {
  kSilent = 0,
  kQuiet,
  kNormal,
  kVerbose,
  kDiagnostic,
  kUserDefined,
};

enum class eClCliPredefValType { kStr, kInt, kFloat, kBool };

///////////////////////////////////////////////////////////////////////////////
/// @end-section Compiler enum applied list definitions.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// @section enum to const char * methods.
///////////////////////////////////////////////////////////////////////////////
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eOpSysToCStr, eOpSys, eOpSys);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eProcArchToCStr, eProcArch, eProcArch);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eToolchainToCStr, eToolchain, eToolchain);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eSrcCharToCStr, eSrcChar, eSrcChar);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eTkToCStr, eTk, eTk);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eCstToCStr, eCst, eCst);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClErrToCStr, eClErr, eClErr);
CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST(eClCliParamToCStr, eClCliParam, eClCliParam);

CND_CX const char* eCharEncodingToCStr(eCharEncoding e) {
  switch (e) {
    case eCharEncoding::kAscii:
      return "kAscii";
    case eCharEncoding::kUtf8:
      return "kUtf8";
    case eCharEncoding::kUtf16:
      return "kUtf16";
    case eCharEncoding::kUtf32:
      return "kUtf32";
    default:
      return "<invalid>";
  }
}

CND_CX const char* eProgLangToCStr(eProgLang e) {
  switch (e) {
    case eProgLang::kAsm:
      return "kAsm";
    case eProgLang::kCand:
      return "kCand";
    case eProgLang::kC:
      return "kC";
    case eProgLang::kCpp:
      return "kCpp";
    default:
      return "<invalid>";
  }
}

CND_CX const char* eAssocToCStr(eAssoc e) {
  switch (e) {
    case eAssoc::INVALID:
      return "kINVALID";
    case eAssoc::NONE:
      return "kNONE";
    case eAssoc::Left:
      return "kLeft";
    case eAssoc::Right:
      return "kRight";
    case eAssoc::COUNT:
      return "kCOUNT";
    default:
      return "<invalid>";
  }
}

CND_CX const char* eOperationToCStr(eOperation e) {
  switch (e) {
    case eOperation::INVALID:
      return "kINVALID";
    case eOperation::NONE:
      return "kNONE";
    case eOperation::Binary:
      return "kBinary";
    case eOperation::Prefix:
      return "kPrefix";
    case eOperation::Postfix:
      return "kPostfix";
    case eOperation::COUNT:
      return "kCOUNT";
    default:
      return "<invalid>";
  }
}

CND_CX const char* ePriorityToCStr(ePriority e) {
  switch (e) {
    case ePriority::INVALID:
      return "kINVALID";
    case ePriority::NONE:
      return "kNONE";
    case ePriority::Assignment:
      return "kAssignment";
    case ePriority::LogicalOr:
      return "kLogicalOr";
    case ePriority::LogicalAnd:
      return "kLogicalAnd";
    case ePriority::BitwiseOr:
      return "kBitwiseOr";
    case ePriority::BitwiseXor:
      return "kBitwiseXor";
    case ePriority::BitwiseAnd:
      return "kBitwiseAnd";
    case ePriority::Equality:
      return "kEquality";
    case ePriority::ThreeWayEquality:
      return "kThreeWayEquality";
    case ePriority::Comparison:
      return "kComparison";
    case ePriority::Bitshift:
      return "kBitshift";
    case ePriority::Term:
      return "kTerm";
    case ePriority::Factor:
      return "kFactor";
    case ePriority::Prefix:
      return "kPrefix";
    case ePriority::Postfix:
      return "kPostfix";
    case ePriority::Functional:
      return "kFunctional";
    case ePriority::Access:
      return "kAccess";
    case ePriority::Max:
      return "kMax";
    default:
      return "<invalid>";
  }
}

CND_CX const char* eClErrTypeToCStr(eClErrType e) {
  switch (e) {
    case eClErrType::kSingle:
      return "kSingle";
    case eClErrType::kChain:
      return "kChain";
    default:
      return "<invalid>";
  }
}

CND_CX const char* eClErrCatToCStr(eClErrCat e) {
  switch (e) {
    case eClErrCat::kNone:
      return "kNone";
    case eClErrCat::kWarning:
      return "kWarning";
    case eClErrCat::kCompilerImpl:
      return "kCompilerImpl";
    case eClErrCat::kGeneral:
      return "kGeneral";
    case eClErrCat::kScanner:
      return "kScanner";
    case eClErrCat::kLexer:
      return "kLexer";
    case eClErrCat::kParser:
      return "kParser";
    case eClErrCat::kPreprocessor:
      return "kPreprocessor";
    case eClErrCat::kConstEval:
      return "kConstEval";
    default:
      return "<invalid>";
  }
}

///////////////////////////////////////////////////////////////////////////////
/// @end-section enum to const char * methods.
///////////////////////////////////////////////////////////////////////////////

// clang-format on

CND_CX eCst eTkToCstEnum(eTk t) {
#define CND_ETK_TO_CST(n) \
  case eTk::k##n:         \
    return eCst::k##n;
  switch (t) {
    CND_APPLIED_ENUM_eTk(CND_ETK_TO_CST, , , case eTk::COUNT : return eCst::COUNT;) default : __assume(false);
  }
#undef CND_ETK_TO_AST
};

CND_CX eClErrCat eClErrToCat(eClErr e) {
  using enum eClErr;
  switch (e) {
    // General
    case kNoError:
    case kCustomError:
    case kFailedToReadFile:
      // case kInvalidCliArg:
      // case kInclusionFailure:
      // case kForbiddenSourceChar:
      // case kLexerUnknownChar:
      // case kMismatchedScope:
      // case kExpectedPragmaticDeclaration:
      // case kExpectedPrimaryExpression:
      // case kNotImplemented:
      // case kInvalidForLoopSyntax:
      // case kUserSyntaxError:
      // case kInvalidSingularOperand:
      // case kParserExpectedToken:
      // case kCevalIntegerOverflow:
      // case kCevalUnsignedOverflow:
      // case kCevalRealOverflow:
      // case kCevalInvalidBoolLiteral:
      // case kCevalInvalidCharLiteral:
      // case kCevalInvalidByteLiteral:
      // case kCevalInvalidStringLiteral:
      // case kInvalidEscapeSequence:
      return eClErrCat::kGeneral;
    // Command Line
    case kInvalidCliArg:
    case kCliFlagMustHavePostfix:
    case kCliFlagExpectedArgs:
    case kCliFlagInvalidArg:
    case kCliDeniedOverwrite:
      return eClErrCat::kCommandLine;
    default:
      return eClErrCat::kNone;
  }
}

}  // namespace cnd

namespace cnd {

///////////////////////////////////////////////////////////////////////////////
// @section Static Asserts to make sure no enum is left unhandled.
///////////////////////////////////////////////////////////////////////////////

// clang-format off
#define CND_STATIC_ASSERT_ENUM_TO_CSTR(x,en,fn)\
    static_assert(cxx::StrEq(fn(en::k##x), CND_MM_IDTOSTR(k##x)));

// eOpSys
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eOpSys(x) \
  CND_STATIC_ASSERT_ENUM_TO_CSTR(x,eOpSys,eOpSysToCStr)

CND_APPLIED_ENUM_eOpSys(CND_STATIC_ASSERT_ENUM_TO_CSTR_eOpSys, , , );
static_assert(cxx::StrEq(eOpSysToCStr(eOpSys::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eOpSys

// eProcArch
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eProcArch(x)
CND_APPLIED_ENUM_eProcArch(CND_STATIC_ASSERT_ENUM_TO_CSTR_eProcArch, , , );
static_assert(cxx::StrEq(eProcArchToCStr(eProcArch::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eProcArch

// eToolchain
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eToolchain(x)
CND_APPLIED_ENUM_eToolchain(CND_STATIC_ASSERT_ENUM_TO_CSTR_eToolchain, , , );
static_assert(cxx::StrEq(eToolchainToCStr(eToolchain::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eToolchain


// eClCliParam
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClCliParam(x)
CND_APPLIED_ENUM_eClCliParam(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClCliParam, , , );
static_assert(cxx::StrEq(eClCliParamToCStr(eClCliParam::COUNT), "COUNT"));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eClCliParam


// eSrcChar
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eSrcChar(x)
CND_APPLIED_ENUM_eSrcChar(CND_STATIC_ASSERT_ENUM_TO_CSTR_eSrcChar, , , );
static_assert(cxx::StrEq(eSrcCharToCStr(eSrcChar::COUNT), "COUNT"));
static_assert(CND_SCAST<char>(eSrcChar::COUNT) == CND_SCAST<char>(128));
#undef CND_STATIC_ASSERT_ENUM_TO_CSTR_eSrcChar

// eTk
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eTk(x)
CND_APPLIED_ENUM_eTk(CND_STATIC_ASSERT_ENUM_TO_CSTR_eTk, , , );
static_assert(cxx::StrEq(eTkToCStr(eTk::COUNT), "COUNT"));

// eCst
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eCst(x)
CND_APPLIED_ENUM_eCst(CND_STATIC_ASSERT_ENUM_TO_CSTR_eCst, , , );
static_assert(cxx::StrEq(eCstToCStr(eCst::COUNT), "COUNT"));

// eClErr
#define CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErr(x)
CND_APPLIED_ENUM_eClErr(CND_STATIC_ASSERT_ENUM_TO_CSTR_eClErr, , , );
static_assert(cxx::StrEq(eClErrToCStr(eClErr::COUNT), "COUNT"));

// clang-format on

///////////////////////////////////////////////////////////////////////////////
// @end-section Static Asserts to make sure no enum is left unhandled.
///////////////////////////////////////////////////////////////////////////////

}  // namespace cnd

// undef file-local macros
#undef CND_MM_CREATE_ENUM_FROM_APPLIED_ENUM
#undef CND_MM_IDENT_TO_CSTR_SWITCH_CASE
#undef CND_MM_CREATE_ENUMTOCSTR_FROM_ENUM_LIST

// These may persist:

// #undef CND_MM_EXPAND
// #undef CND_MM_COMMA
// #undef CND_MM_IDTOSTR
// #undef CND_MM_CONST_ID_TAG

// #undef CND_APPLIED_ENUM_eOpSys
// #undef CND_APPLIED_ENUM_eProcArch
// #undef CND_APPLIED_ENUM_eToolchain
// #undef CND_APPLIED_ENUM_eSrcChar
// #undef CND_APPLIED_ENUM_eTk
// #undef CND_APPLIED_ENUM_eCst
// #undef CND_APPLIED_ENUM_eClErr