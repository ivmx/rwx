/*
 * wxTextArea.cpp
 *
 *  Created on: 30.12.2013
 *      Author: hanmac
 */

#include "wxTextArea.hpp"
#include "wxTextAttr.hpp"
#include "wxTextCtrl.hpp"
#include "wxRichTextCtrl.hpp"
#include "wxSearchCtrl.hpp"
#include "wxSTC.hpp"

VALUE rb_mWXTextArea;

#if wxUSE_TEXTCTRL

template <>
wxTextAreaBase* unwrap< wxTextAreaBase* >(const VALUE &obj)
{
	if(rb_obj_is_kind_of(obj,rb_cWXTextCtrl))
		return unwrap<wxTextCtrl*>(obj);
#if	wxUSE_RICHTEXT
	if(rb_obj_is_kind_of(obj,rb_cWXRichTextCtrl))
		return unwrap<wxRichTextCtrl*>(obj);
#endif
#if	wxUSE_SEARCHCTRL
	if(rb_obj_is_kind_of(obj,rb_cWXSearchCtrl))
		return unwrap<wxSearchCtrl*>(obj);
#endif
#if	wxUSE_STC
	if(rb_obj_is_kind_of(obj,rb_cWXSTC))
		return unwrap<wxStyledTextCtrl*>(obj);
#endif
	rb_raise(rb_eTypeError,"Unknown wxTextAreaBase: %s",rb_obj_classname(obj));
return NULL;
}

#define _self unwrap<wxTextAreaBase*>(self)

namespace RubyWX {
namespace TextArea {

singlereturn(GetNumberOfLines)

macro_attr_bool(Modified)

macro_attr(DefaultStyle,wxTextAttr)


/*
 * call-seq:
 *   initialize(parent, [options])
 *
 *
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);

	rb_call_super(argc,argv);

#if	wxUSE_STC
	if(rb_obj_is_kind_of(self,rb_cWXSTC))
		return self;
#endif

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_obj_option(hash, "modified", &wxTextAreaBase::SetModified,_self);
		set_obj_option(hash, "default_style", &wxTextAreaBase::SetDefaultStyle,_self);
	}

	return self;
}


DLL_LOCAL VALUE _each_line(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_GetNumberOfLines));

	for(int i = 0; i < _self->GetNumberOfLines(); ++i)
	{
		rb_yield(wrap(_self->GetLineText(i)));
	}

	return self;
}

DLL_LOCAL VALUE _load_file(VALUE self,VALUE file)
{
	if(!check_file_loadable(unwrap<wxString>(file)))
		return Qfalse;
	return wrap(_self->LoadFile(unwrap<wxString>(file)));
}

DLL_LOCAL VALUE _save_file(VALUE self,VALUE file)
{
	if(!check_file_saveable(unwrap<wxString>(file)))
		return Qfalse;

	return wrap(_self->SaveFile(unwrap<wxString>(file)));
}


}
}

#endif

DLL_LOCAL void Init_WXTextArea(VALUE rb_mWX)
{
#if wxUSE_TEXTCTRL
	using namespace RubyWX::TextArea;
	rb_mWXTextArea = rb_define_module_under(rb_mWX,"TextArea");
	rb_define_method(rb_mWXTextArea,"each_line",RUBY_METHOD_FUNC(_each_line),0);

#if 0
	rb_define_attr(rb_mWXTextArea,"modified",1,1);
	rb_define_attr(rb_mWXTextArea,"default_style",1,1);
#endif

	rb_define_method(rb_mWXTextArea,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_mWXTextArea,"modified",_getModified,_setModified);
	rb_define_attr_method(rb_mWXTextArea,"default_style",_getDefaultStyle,_setDefaultStyle);

	rb_define_method(rb_mWXTextArea,"load_file",RUBY_METHOD_FUNC(_load_file),1);
	rb_define_method(rb_mWXTextArea,"save_file",RUBY_METHOD_FUNC(_save_file),1);
#endif

}



