// message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>

class message
{
public:
	enum { header_length = 22 };
	enum { max_body_length = 1024 };

	message()
		: body_length_(0)
	{
	}

	const char* data() const;

	char* data();
	size_t max_length();
	size_t length() const;

	const char* body() const;

	char* body();

	size_t body_length() const;

	void body_length(size_t length);
	bool decode_header();

	void encode_header();

private:
	char data_[header_length + max_body_length];
	size_t body_length_;
};