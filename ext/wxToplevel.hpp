/*
 * wxToplevel.hpp
 *
 *  Created on: 05.02.2012
 *      Author: hanmac
 */

#ifndef WXTOPLEVEL_HPP_
#define WXTOPLEVEL_HPP_

#include "wxWindow.hpp"

extern VALUE rb_cWXTopLevel;
DLL_LOCAL void Init_WXTopLevel(VALUE rb_mWX);

namespace RubyWX {
namespace TopLevel {

void set_style_flags(VALUE hash,int& flags);

}
}


#endif /* WXTOPLEVEL_HPP_ */
