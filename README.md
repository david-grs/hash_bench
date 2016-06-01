Benchmark again different hash map implementations. Not that many right now, more coming soon.

```
Test                                                                                                                                                    Time (ns)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
std::set<std::string, std::less<std::string>, std::allocator<std::string> >                                                                            47,531,293
std::unordered_set<std::string, std::hash<std::string>, std::equal_to<std::string>, std::allocator<std::string> >                                      11,530,360
google::dense_hash_set<std::string, std::tr1::hash<std::string>, std::equal_to<std::string>, google::libc_allocator_with_realloc<std::string> >        10,173,628
hot_set<std::string, variable<std::string>, std::equal_to<void>, std::allocator<std::string>, std::hash<std::string>, default_load_policy>              9,343,105
ht_chained<std::string>                                                                                                                                19,619,610
rigtorp::HashMap<std::string, int, std::hash<std::string> >                                                                                            13,070,759
```

PS: std::set is not a hash, I know ; it is here for comparison purposes.

