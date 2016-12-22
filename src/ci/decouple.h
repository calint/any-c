#pragma once
inline static bool ci_is_type_builtin(cstr);
//struct xtype;
inline static struct xtype*ci_get_type_by_name(const toc*o,cstr);
inline static void ci_xset_assert(const toc*,cstr,cstr,token);
//inline static cstr ci_get_type_for_accessor(const toc*,cstr,token);
inline static /*gives*/xexp*ci_read_next_statement(toc*o);
inline static /*gives*/xexp*ci_read_next_expression(toc*);
inline static void ci_xcall_compile(toc*,token,cstr,unsigned);
inline static void ci_xset_compile(const toc*,token,cstr,cstr);
inline static void ci_xcode_compile_free_current_scope();
inline static void ci_xcode_compile_free_current_loop_scope(toc*,token);
//inline static bool ci_xcode_needs_compile_free_current_loop_scope(toc*,token);
inline static bool ci_type_needs_init(toc*,cstr);
inline static bool ci_is_func_arg_ref(toc*,token,cstr,unsigned);
struct xcall;
inline static void ci_xcall_assert(const toc*,struct xcall*);



struct xreturn;
inline static void ci_xreturn_assert(const toc*tc,struct xreturn*);



struct xaccessorinfo{
	cstr type;
	bool is_ref;
};
inline static struct xaccessorinfo ci_get_accessorinfo(
		toc*tc,token tk,cstr accessor);
