#pragma once

inline static /*gives*/xexp*ci_read_next_statement(toc*);
inline static /*gives*/xexp*ci_read_next_expression(toc*);

typedef struct xtyperef{
	cstr type;
	bool is_ref;
}xtyperef;
inline static xtyperef ci_get_typeref_for_accessor(const toc*,token,cstr);
inline static bool ci_is_builtin_type(cstr);
inline static bool ci_is_func_arg_ref(const toc*,token,cstr,unsigned);


inline static struct xtype*ci_get_type_by_name_try(const toc*,cstr);
struct xcall;
struct xset;
struct xreturn;
inline static void ci_xcall_compile(const toc*,const struct xcall*);
inline static void ci_xset_assert(const toc*,const struct xset*);
inline static void ci_xset_compile(const toc*tc,const struct xset*o);
inline static void ci_xcode_compile_free_current_scope();
inline static void ci_xcode_compile_free_current_loop_scope(const toc*,token);
inline static bool ci_xvar_needs_init(const toc*,cstr);
inline static void ci_xcall_assert(const toc*,struct xcall*);
inline static void ci_xreturn_assert(const toc*tc,struct xreturn*);

