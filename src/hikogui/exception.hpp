// Copyright Take Vos 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <exception>
#include <stdexcept>

#pragma once

namespace hi::inline v1 {

/** Exception thrown during parsing on an error.
 * This exception is often thrown due to an error in the syntax
 * in both text and binary files.
 *
 * The what-string should start with the location of the error in the file followed with ": " and the error message.
 * The what-string may be shown to the user, when the parser was working on user supplied files.
 *
 * The location for a text file will be: a path followed by line_nr (starting at line 1) and column_nr (starting at column 1).
 * The location for a binary: a path followed by an optional chunk names, followed by a byte number within the chunk.
 *
 * If there are nested errors, such as an error in an included file, then the what-string may be multiple-lines, where the
 * nested error appears later in the what-string.
 */
class parse_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/** Exception thrown during execution of a dynamic operation.
 * This exception is often thrown on operation between multiple polymorphic objects
 * which do not support the combined operation.
 *
 * For example a datum object may contain floating point number for which
 * a shift-right or shift-left would be an invalid operation.
 */
class operation_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/** Exception thrown during I/O on an error.
 * This exception is often thrown due to an error with permission or existence of files.
 *
 * The what-string should start with the path of the object where the error happened.
 * Followed after ": " with a user-friendly error message. Optionally followed between single quotes
 * the operating system error string.
 */
class io_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/** Exception thrown during an operating system call.
 * This exception is often thrown due to an error with permission or incorrect given parameters
 *
 * The what-string should start with a user-friendly error message.
 * Optionally followed between single quotes the operating system error string.
 */
class os_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class gui_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class key_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class url_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

/** Cancel error is caused by user pressing cancel.
 * Cancels can be cause by a local user pressing cancel in a dialog box,
 * or by a remote user through a network connection.
 */
class cancel_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

} // namespace hi::inline v1