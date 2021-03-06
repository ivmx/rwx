/*
 * wxRealPoint.cpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#include "wxPoint.hpp"
VALUE rb_cwxPoint;

ID rwxID_x,rwxID_y;

#define _self unwrap<wxRealPoint*>(self)


template <>
VALUE wrap< wxRealPoint >(wxRealPoint *point )
{
	return wrapTypedPtr(point, rb_cwxPoint);
}

template <>
VALUE wrap< wxPoint >(const wxPoint &point )
{
	return wrap(new wxRealPoint(point));
}

template <>
bool is_wrapable< wxRealPoint >(const VALUE &vpoint)
{
	if (rb_obj_is_kind_of(vpoint, rb_cwxPoint)){
		return true;
	}else if(rb_respond_to(vpoint,rwxID_x) &&
		rb_respond_to(vpoint,rwxID_y)){
		return true;
	}else
		return false;
}
template <>
bool is_wrapable< wxPoint >(const VALUE &vpoint)
{
	return is_wrapable< wxRealPoint >(vpoint);
}

template <>
wxRealPoint unwrap< wxRealPoint >(const VALUE &vpoint)
{
	if(rb_obj_is_kind_of(vpoint, rb_cArray) && RARRAY_LEN(vpoint) == 2){
		wxRealPoint point;
		point.x = NUM2DBL(RARRAY_AREF(vpoint,0));
		point.y = NUM2DBL(RARRAY_AREF(vpoint,1));
		return point;
	}else if(rb_obj_is_kind_of(vpoint, rb_cHash)){
		wxRealPoint point;
		point.x = NUM2DBL(rb_hash_aref(vpoint,RB_ID2SYM(rwxID_x)));
		point.y = NUM2DBL(rb_hash_aref(vpoint,RB_ID2SYM(rwxID_y)));
		return point;
	}else if(!rb_obj_is_kind_of(vpoint, rb_cwxPoint) &&
		rb_respond_to(vpoint,rwxID_x) &&
		rb_respond_to(vpoint,rwxID_y)){
		wxRealPoint point;
		point.x = NUM2DBL(rb_funcall(vpoint,rwxID_x,0));
		point.y = NUM2DBL(rb_funcall(vpoint,rwxID_y,0));
		return point;
	}else{
		return *unwrap<wxRealPoint*>(vpoint);
	}

}


template <>
wxPoint unwrap< wxPoint >(const VALUE &vpoint)
{
	if(rb_obj_is_kind_of(vpoint, rb_cArray) && RARRAY_LEN(vpoint) == 2){
		wxPoint point;
		point.x = RB_NUM2INT(RARRAY_AREF(vpoint,0));
		point.y = RB_NUM2INT(RARRAY_AREF(vpoint,1));
		return point;
	}else if(rb_obj_is_kind_of(vpoint, rb_cHash)){
		wxPoint point;
		point.x = RB_NUM2INT(rb_hash_aref(vpoint,RB_ID2SYM(rwxID_x)));
		point.y = RB_NUM2INT(rb_hash_aref(vpoint,RB_ID2SYM(rwxID_y)));
		return point;
	}else if(!rb_obj_is_kind_of(vpoint, rb_cwxPoint) &&
		rb_respond_to(vpoint,rwxID_x) &&
		rb_respond_to(vpoint,rwxID_y)){
		wxPoint point;
		point.x = RB_NUM2INT(rb_funcall(vpoint,rwxID_x,0));
		point.y = RB_NUM2INT(rb_funcall(vpoint,rwxID_y,0));
		return point;
	}else{
		return *unwrap<wxRealPoint*>(vpoint);
	}

}

template <>
wxPointList* unwrap< wxPointList* >(const VALUE &val)
{
	wxPointList *result = new wxPointList;
	VALUE dp(rb_Array(val));
	std::size_t length = RARRAY_LEN(dp);
	for(std::size_t i = 0; i < length; ++i)
		result->Append(new wxPoint(unwrap<wxPoint>(RARRAY_AREF(dp,i))));
	return result;
}

namespace RubyWX {
namespace Point {

macro_attr_prop(x,double)
macro_attr_prop(y,double)

DLL_LOCAL VALUE _alloc(VALUE self)
{
	return wrapTypedPtr(new wxRealPoint,self);
}

/*
 * call-seq:
 *   Point.new(x, y)
 *
 * Creates a new Point object.
*/
DLL_LOCAL VALUE _initialize(VALUE self,VALUE x,VALUE y)
{
	_set_x(self,x);
	_set_y(self,y);
	return self;
}

/* Document-method: initialize_copy
 * call-seq:
 *   initialize_copy(orig)
 *
 * Duplicate an object
*/
DLL_LOCAL VALUE _initialize_copy(VALUE self, VALUE other)
{
	VALUE result = rb_call_super(1,&other);
	_set_x(self,_get_x(other));
	_set_y(self,_get_y(other));
	return result;
}

/* Document-method: inspect
 * call-seq:
 *   inspect -> String
 *
 * Human-readable description.
 * ===Return value
 * String
*/
DLL_LOCAL VALUE _inspect(VALUE self)
{
	return rb_sprintf( "%s(%f, %f)",
		rb_obj_classname( self ),
		NUM2DBL(_get_x(self)),
		NUM2DBL(_get_y(self)));
}

/* Document-method: marshal_dump
 * call-seq:
 *   marshal_dump -> Array
 *
 * Provides marshalling support for use by the Marshal library.
 * ===Return value
 * Array
 */
DLL_LOCAL VALUE _marshal_dump(VALUE self)
{
    VALUE ptr[2];
    ptr[0] = _get_x(self);
    ptr[1] = _get_y(self);
    return rb_ary_new4(2, ptr);
}

/* Document-method: marshal_load
 * call-seq:
 *   marshal_load(array) -> nil
 *
 * Provides marshalling support for use by the Marshal library.
 *
 *
 */
DLL_LOCAL VALUE _marshal_load(VALUE self, VALUE data)
{
    data = rb_Array(data);
    _set_x(self, RARRAY_AREF(data,0));
    _set_y(self, RARRAY_AREF(data,1));
    return Qnil;
}

/*
 * call-seq:
 *   hash -> Fixnum
 *
 * Generates a Fixnum hash value for this object.
 *
 *
 */
DLL_LOCAL VALUE _getHash(VALUE self)
{
	st_index_t h = rb_hash_start(0);

	h = rb_hash_uint(h, RB_NUM2LONG(rb_hash(_get_x(self))));
	h = rb_hash_uint(h, RB_NUM2LONG(rb_hash(_get_y(self))));

	h = rb_hash_end(h);
	return RB_LONG2FIX(h);
}


struct equal_obj {
	wxRealPoint* self;
	VALUE other;
};

VALUE _equal_block(equal_obj *obj)
{
	return wrap(*obj->self == unwrap<wxRealPoint>(obj->other));
}

/*
 * call-seq:
 *   == point -> bool
 *
 * compares two points.
 *
 *
 */
DLL_LOCAL VALUE _equal(VALUE self, VALUE other)
{
	equal_obj obj;
	obj.self = _self;
	obj.other = other;

	return rb_rescue(
		RUBY_METHOD_FUNC(_equal_block),(VALUE)&obj,
		RUBY_METHOD_FUNC(_equal_rescue),Qnil
	);
}



}
}



/* Document-class: WX::Point
 *
 * This class represents an Point.
 */

/* Document-attr: x
 * returns the x value of Point.
 */
/* Document-attr: y
 * returns the y value of Point.
 */

/**/
DLL_LOCAL void Init_WXPoint(VALUE rb_mWX)
{

	using namespace RubyWX::Point;
	rb_cwxPoint = rb_define_class_under(rb_mWX,"Point",rb_cObject);

	rb_define_alloc_func(rb_cwxPoint,_alloc);

#if 0
	rb_define_attr(rb_cwxPoint,"x",1,1);
	rb_define_attr(rb_cwxPoint,"y",1,1);
#endif

	rb_define_method(rb_cwxPoint,"initialize",RUBY_METHOD_FUNC(_initialize),2);
	rb_define_private_method(rb_cwxPoint,"initialize_copy",RUBY_METHOD_FUNC(_initialize_copy),1);

	rb_define_attr_method(rb_cwxPoint,"x",_get_x,_set_x);
	rb_define_attr_method(rb_cwxPoint,"y",_get_y,_set_y);

	rb_define_method(rb_cwxPoint,"inspect",RUBY_METHOD_FUNC(_inspect),0);

	rb_define_method(rb_cwxPoint,"==",RUBY_METHOD_FUNC(_equal),1);
	rb_define_alias(rb_cwxPoint,"eql?","==");

	rb_define_method(rb_cwxPoint,"hash",RUBY_METHOD_FUNC(_getHash),0);

	rb_define_method(rb_cwxPoint,"marshal_dump",RUBY_METHOD_FUNC(_marshal_dump),0);
	rb_define_method(rb_cwxPoint,"marshal_load",RUBY_METHOD_FUNC(_marshal_load),1);

	registerType<wxRealPoint>(rb_cwxPoint, true);

	rwxID_x = rb_intern("x");
	rwxID_y = rb_intern("y");
}



