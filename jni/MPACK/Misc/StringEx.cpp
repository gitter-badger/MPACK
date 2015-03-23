#include "StringEx.hpp"

using namespace std;

namespace MPACK
{
	namespace Core
	{
		namespace StringEx
		{
			const char* GetExtension(const char* pStr)
			{
				const char* last=NULL;
				while(pStr)
				{
					++pStr;
					if(*pStr=='.')
					{
						last=pStr;
					}
				}
				if(!last)
				{
					last=pStr;
				}
				else
				{
					++last;
				}
				return last;
			}

			void GetExtension(string path, string &ext)
			{
				int i;
				ext="";
				for(i=path.size()-1;i>=0;--i)
				{
					if(path[i]=='.')
					{
						break;
					}
				}
				if(i>=0)
				{
					++i;
					for(;i<path.size();++i)
					{
						ext=ext+path[i];
					}
				}
			}

			string& Upper(string &str)
			{
				for(int i=0;i<str.size();++i)
				{
					if('a'<=str[i] && str[i]<='z')
					{
						str[i]+='A'-'a';
					}
				}
				return str;
			}

			string& Lower(string &str)
			{
				for(int i=0;i<str.size();++i)
				{
					if('A'<=str[i] && str[i]<='Z')
					{
						str[i]+='a'-'A';
					}
				}
				return str;
			}

			string ToUpper(std::string str)
			{
				for(int i=0;i<str.size();++i)
				{
					if('a'<=str[i] && str[i]<='z')
					{
						str[i]=str[i]-'a'+'A';
					}
				}
				return str;
			}

			string ToLower(std::string str)
			{
				for(int i=0;i<str.size();++i)
				{
					if('A'<=str[i] && str[i]<='Z')
					{
						str[i]=str[i]-'A'+'a';
					}
				}
				return str;
			}

			char& Upper(char &ch)
			{
				if('a'<=ch && ch<='z')
				{
					ch=ch-'a'+'A';
				}
				return ch;
			}

			char& Lower(char &ch)
			{
				if('A'<=ch && ch<='Z')
				{
					ch=ch-'A'+'a';
				}
				return ch;
			}

			char ToUpper(const char &ch)
			{
				if('a'<=ch && ch<='z')
				{
					return ch-'a'+'A';
				}
				return ch;
			}

			char ToLower(const char &ch)
			{
				if('A'<=ch && ch<='Z')
				{
					return ch-'A'+'a';
				}
			}

			bool IsLowercase(const char &ch)
			{
				return 'a'<=ch && ch<='z';
			}

			bool IsUppercase(const char &ch)
			{
				return 'A'<=ch && ch<='Z';
			}

			bool IsNumeric(const char &ch)
			{
				return '0'<=ch && ch<='9';
			}

			bool IsAlphaNumeric(const char &ch)
			{
				return ('0'<=ch && ch<='9') || ('a'<=ch && ch<='z') || ('A'<=ch && ch<='Z');
			}

			bool IsWhiteSpace(const char &ch)
			{
				return ch==' ' || ch=='\t' || ch=='\n' || ch=='\r';
			}

			string StripLeft(string &str)
			{
				string ans;
				int i=0;
				for(;i<str.size();++i)
				{
					if(!IsWhiteSpace(str[i]))
					{
						break;
					}
				}
				for(;i<str.size();++i)
				{
					ans+=str[i];
				}
				return ans;
			}

			string StripRight(string &str)
			{
				int i=str.size()-1;
				for(;i>=0 && IsWhiteSpace(str[i]);--i);
				if(IsWhiteSpace(str[i]))
				{
					--i;
				}
				string ans=str;
				ans.resize(i+1);
				return ans;
			}

			string Strip(string &str)
			{
				string ans;
				int i=0;
				for(;i<str.size();++i)
				{
					if(!IsWhiteSpace(str[i]))
					{
						break;
					}
				}
				for(;i<str.size();++i)
				{
					ans+=str[i];
				}

				i=ans.size()-1;
				for(;i>=0 && IsWhiteSpace(ans[i]);--i);
				if(IsWhiteSpace(ans[i]))
				{
					--i;
				}
				ans.resize(i+1);
				return ans;
			}


			string Substr(string &str, int start, int length)
			{
				string ans;
				int end;
				if(length==-1)
				{
					end=str.size();
				}
				else
				{
					end=start+length-1;
				}
				if(end>=str.size())
				{
					end=str.size()-1;
				}
				for(int i=start;i<=end;++i)
				{
					ans+=str[i];
				}
				return ans;
			}
		}
	}
}
