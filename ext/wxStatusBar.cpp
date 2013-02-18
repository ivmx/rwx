/*
 * wxStatusBar.cpp
 *
 *  Created on: 26.02.2012
 *      Author: hanmac
 */


#include "wxWindow.hpp"


VALUE rb_cWXStatusBar;

#if wxUSE_STATUSBAR
#define _self unwrap<wxStatusBar*>(self)

namespace RubyWX {
namespace StatusBar {

APP_PROTECT(wxStatusBar)


macro_attr(StatusText,wxString)
macro_attr(FieldsCount,int)


VALUE _getStatusText2(VALUE self,VALUE num)
{
	if(NUM2INT(num) < _self->GetFieldsCount())
		return wrap(_self->GetStatusText(NUM2INT(num)));
	return Qnil;
}

VALUE _setStatusText2(VALUE self,VALUE str,VALUE num)
{
	if(NUM2INT(num) < _self->GetFieldsCount())
		_self->SetStatusText(unwrap<wxString>(str),NUM2INT(num));
	//const_cast<wxStatusBarPane&>(_self->GetField(NUM2INT(num))).SetText(unwrap<wxString>(str));
	//_self->Update();
	return Qnil;
}

VALUE _getStatusWidth(VALUE self,VALUE num)
{
	if(NUM2INT(num) < _self->GetFieldsCount())
		return INT2NUM(_self->GetStatusWidth(NUM2INT(num)));
	return Qnil;
}

VALUE _setStatusWidth(VALUE self,VALUE val,VALUE num)
{
	int w = NUM2INT(val);
	if(NUM2INT(num) < _self->GetFieldsCount())
		_self->SetStatusWidths(NUM2INT(num),&w);
	return Qnil;
}

VALUE _pushStatusText(int argc,VALUE *argv,VALUE self)
{
	VALUE str,num;
	rb_scan_args(argc, argv, "11",&str,&num);

	if(NIL_P(num))
		_self->PushStatusText(unwrap<wxString>(str));
	else
		_self->PushStatusText(unwrap<wxString>(str),NUM2INT(num));
	return Qnil;
}

VALUE _popStatusText(int argc,VALUE *argv,VALUE self)
{
	VALUE num;
	rb_scan_args(argc, argv, "01",&num);

	if(NIL_P(num))
		_self->PopStatusText();
	else
		_self->PopStatusText(NUM2INT(num));
	return Qnil;
}



namespace Pane
{
#undef _self
#define _self unwrap<wxStatusBarPane*>(self)

macro_attr(Width,int)
macro_attr(Style,int)
macro_attr(Text,wxString)

VALUE _pushText(VALUE self,VALUE str)
{
	return wrap(_self->PushText(unwrap<wxString>(str)));
}

singlereturn(PopText)


}

}
}
#endif
DLL_LOCAL void Init_WXStatusBar(VALUE rb_mWX)
{
#if wxUSE_STATUSBAR
	using namespace RubyWX::StatusBar;
	rb_cWXStatusBar = rb_define_class_under(rb_mWX,"StatusBar",rb_cWXControl);
	rb_define_alloc_func(rb_cWXStatusBar,_alloc);

	rb_define_attr_method(rb_cWXStatusBar,"status_text",_getStatusText,_setStatusText);
	rb_define_attr_method(rb_cWXStatusBar,"fields_count",_getFieldsCount,_setFieldsCount);

	rb_define_method(rb_cWXStatusBar,"get_status_text",RUBY_METHOD_FUNC(_getStatusText2),1);
	rb_define_method(rb_cWXStatusBar,"set_status_text",RUBY_METHOD_FUNC(_setStatusText2),2);

	rb_define_method(rb_cWXStatusBar,"get_status_width",RUBY_METHOD_FUNC(_getStatusWidth),1);
	rb_define_method(rb_cWXStatusBar,"set_status_width",RUBY_METHOD_FUNC(_setStatusWidth),2);

	rb_define_method(rb_cWXStatusBar,"push_status_text",RUBY_METHOD_FUNC(_pushStatusText),-1);
	rb_define_method(rb_cWXStatusBar,"pop_status_text",RUBY_METHOD_FUNC(_popStatusText),-1);


	registerInfo<wxStatusBar>(rb_cWXStatusBar);
#endif

}




