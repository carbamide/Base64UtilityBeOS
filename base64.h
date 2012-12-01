//
//  base64.h
//  Base64Utility
//
//  Created by Josh Barrow on 3/14/12.
//  Copyright 2012 Jukaela Enterprises. All rights reserved.
//

#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);
