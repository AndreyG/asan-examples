CC=clang++ -fsanitize=address -g -std=c++17

stack-buffer-overflow: stack-buffer-overflow.cpp
	$(CC) stack-buffer-overflow.cpp -o stack-buffer-overflow

heap-buffer-overflow: heap-buffer-overflow.cpp
	$(CC) heap-buffer-overflow.cpp -o heap-buffer-overflow

polymorphic-key: polymorphic-key.cpp
	$(CC) polymorphic-key.cpp -o polymorphic-key

string-view-as-dict-key: string-view-as-dict-key.cpp
	g++ -fsanitize=address -g -std=c++17 string-view-as-dict-key.cpp -o string-view-as-dict-key

ranges: ranges.cpp
	$(CC) ranges.cpp -o ranges

fuzz_me: fuzz_me.cpp
	clang++ -fsanitize=address,fuzzer -g fuzz_me.cpp -o fuzz_me 

