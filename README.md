# Worldwide Argument Parsing Library
**wapl** is a C library that provides a consistent interface and look and feel to CLI applications.
It includes an argument parser built-in, but also includes useful extras to make every CLI app that
uses this library feel and function similarly.

Features of **wapl** include:
* Easy to make language bindings for for other programming langauges, in particuilar Rust.
* Extremely portable; can run on both modern and old POSIX systems, modern and old versions of
  Windows, and even some more obscure stuff like DOS.
* Written in ANSI C to assist with the above further - but is optionally  enhanced with features from C99+.
* Includes a simple-to-use argument parser.
* Includes data types for collecting a bunch of errors into a single object and then printing the
  error out all at once.
* No build system; the library is a single C file that includes all the other files in the project.
  This makes it easy to integrate into projects and more portable.
* Easily control the color scheme of the CLI app.

