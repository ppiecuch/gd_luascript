/*
   MIT License

   Copyright (c) 2021 Jordan Vrtanoski

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   */

#include <memory>
#include <stdexcept>

#include "LuaRegistry.hpp"
#include "LuaCompiler.hpp"
#include "LuaLibrary.hpp"
#include "LuaCodeSnippet.hpp"
#include "LuaCFunction.hpp"
#include "../Engine/LuaState.hpp"


using namespace LuaCpp::Engine;
using namespace LuaCpp::Registry;

void LuaRegistry::CompileAndAddString(const std::string &name, const std::string &code) {
	CompileAndAddString(name, code, false);
}

void LuaRegistry::CompileAndAddString(const std::string &name, const std::string &code, bool recompile) {

	if ( !Exists(name) or recompile ) { 
		LuaCompiler cmp;
		std::unique_ptr<LuaCodeSnippet> snp = cmp.CompileString(name, code);

		registry[name] = std::move(*snp);
	}
}

void LuaRegistry::CompileAndAddFile(const std::string &name, const std::string &fname) {
	CompileAndAddFile(name, fname, false);
}

void LuaRegistry::CompileAndAddFile(const std::string &name, const std::string &fname, bool recompile) {

	if ( !Exists(name) or recompile ) { 
		LuaCompiler cmp;
		std::unique_ptr<LuaCodeSnippet> snp = cmp.CompileFile(name, fname);

		registry[name] = std::move(*snp);
	}
}

std::unique_ptr<LuaCodeSnippet> LuaRegistry::getByName(const std::string &name) {
	return std::make_unique<LuaCodeSnippet>(registry[name]);
}


// LuaLibrary

std::string LuaLibrary::getName() {
	return name;
}

void LuaLibrary::setName(const std::string &_name) {
	name = std::move(_name);
}

void LuaLibrary::AddCFunction(const std::string &name, lua_CFunction cfunction) {
	AddCFunction(name, cfunction, false);
}

void LuaLibrary::AddCFunction(const std::string &name, lua_CFunction cfunction, bool replace) {
	if (!Exists(name) or replace) {
		std::unique_ptr<LuaCFunction> func = std::make_unique<LuaCFunction>(cfunction);
		func->setName(name);
		functions.insert(std::make_pair(name, std::move(*func)));
	}
}

int LuaLibrary::RegisterFunctions(LuaState &L) {

	luaL_Reg reg[functions.size()+1];
	int count = 0;

	for (auto & x : functions) {
		LuaCFunction& lcf = x.second;
		 reg[count].name = x.first.c_str();
		reg[count].func = lcf.getCFunction();
		count++;
	}

	reg[count].name = NULL;
	reg[count].func = NULL;

	luaL_newlib(L, reg);
	lua_setglobal(L,name.c_str());

	return 0;
}


// LuaCompiler

void inline _checkErrorAndThrow(LuaState &L, int error) {
	if (error != LUA_OK) {
		switch (error) {
			case LUA_ERRMEM:
				throw std::runtime_error("Out of memory");
				break;
			case LUA_ERRGCMM:
				throw std::out_of_range("GC Error while loading");
				break;
			case LUA_ERRSYNTAX:
				throw std::logic_error(lua_tostring(L,1));
				break;
			default:
				throw std::runtime_error("Unknown error code "+ std::to_string(error) + " :" +lua_tostring(L,1));
		}
	}
}

std::unique_ptr<LuaCodeSnippet> LuaCompiler::CompileString(std::string name, std::string code) {
	std::unique_ptr<LuaCodeSnippet> cb_ptr = std::make_unique<LuaCodeSnippet>();

	LuaState L;
	int res = luaL_loadstring(L.getState(), code.c_str());
	_checkErrorAndThrow(L, res);

	res = lua_dump(L.getState(), code_writer, (void*) cb_ptr.get(), 0);
	_checkErrorAndThrow(L, res);

	cb_ptr->setName(name);
	return cb_ptr;
}

std::unique_ptr<LuaCodeSnippet> LuaCompiler::CompileFile(std::string name, std::string fname) {
	std::unique_ptr<LuaCodeSnippet> cb_ptr = std::make_unique<LuaCodeSnippet>();

	LuaState L;
	int res = luaL_loadfile(L, fname.c_str());
	_checkErrorAndThrow(L, res);

	res = lua_dump(L, code_writer, (void*) cb_ptr.get(), 0);
	_checkErrorAndThrow(L, res);

	cb_ptr->setName(name);
	return cb_ptr;
}


// LuaCFunction

void LuaCFunction::setName(std::string _name) {
	name = std::move(_name);
}

std::string LuaCFunction::getName() {
	return name;
}

void LuaCFunction::setCFunction(lua_CFunction _cfunction) {
	//  typedef int (*lua_CFunction) (lua_State *L);
	cfunction = _cfunction;
}

lua_CFunction LuaCFunction::getCFunction() {
	return cfunction;
}


// LuaCodeSnippet


LuaCodeSnippet::LuaCodeSnippet() : code() {
	code.clear();
}

int LuaCodeSnippet::WriteCode(unsigned char* buff, size_t size) {
	unsigned char *end = (unsigned char *)buff+size;
	try {
		code.insert(code.end(),buff,end);
		return 0;
	} catch (...) {
		return 1;
	}
}

int LuaCodeSnippet::getSize() {
	return code.size();
}

void LuaCodeSnippet::setName(std::string _name) {
	name = std::move(_name);
}

std::string LuaCodeSnippet::getName() {
	return name;
}

const char *LuaCodeSnippet::getBuffer() {
	return (const char *)&code[0];
}

void LuaCodeSnippet::UploadCode(LuaState &L) {
	lua_load(L, code_reader, this, (const char *)name.c_str(), NULL);
}

int code_writer (lua_State* L, const void* p, size_t size, void* u) {
	return ((LuaCodeSnippet *) u)->WriteCode((unsigned char*)p, size);
}

const char * code_reader (lua_State *L, void *data, size_t *size) {
	*size = ((LuaCodeSnippet *)data)->getSize();
	return ((LuaCodeSnippet *)data)->getBuffer();
}
