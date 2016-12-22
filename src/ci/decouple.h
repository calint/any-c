#pragma once
#include"../lib.h"
struct toc;
struct xexp;
inline static /*gives*/struct xexp*ci_read_next_statement(struct toc*);
inline static /*gives*/struct xexp*ci_read_next_expression(struct toc*);

typedef struct xtyperef{
	cstr type;
	bool is_ref;
}xtyperef;
inline static xtyperef ci_get_typeref_for_accessor(
		const struct toc*,token,cstr);
inline static bool ci_is_builtin_type(cstr);
inline static bool ci_is_func_arg_ref(const struct toc*,token,cstr,unsigned);


inline static struct xtype*ci_get_type_for_name_try(const struct toc*,cstr);
struct xcall;
struct xset;
struct xreturn;
inline static void ci_xcall_compile(const struct toc*,const struct xcall*);
inline static void ci_xset_assert(const struct toc*,const struct xset*);
inline static void ci_xset_compile(const struct toc*tc,const struct xset*o);
inline static void ci_xcode_compile_free_current_scope();
inline static void ci_xcode_compile_free_current_loop_scope(
		const struct toc*,token);
inline static bool ci_xvar_needs_init(const struct toc*,cstr);
inline static void ci_xcall_assert(const struct toc*,struct xcall*);
inline static void ci_xreturn_assert(const struct toc*tc,struct xreturn*);

