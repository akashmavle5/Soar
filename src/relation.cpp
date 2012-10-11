#include "relation.h"
#include "serialize.h"

using namespace std;

struct sliced_relation_tuple {
	tuple match;
	tuple extend;
	const std::set<int> *lead;
}; 

void slice_tuple(const tuple &t1, const tuple &inds, tuple &t2) {
	int n = inds.size();
	t2.clear();
	t2.resize(inds.size());
	for (int i = 0; i < n; ++i) {
		t2[i] = t1[inds[i]];
	}
}

tuple concat_tuples(const tuple &t1, const tuple &t2) {
	tuple t3(t1.begin(), t1.end());
	t3.insert(t3.end(), t2.begin(), t2.end());
	return t3;
}

relation::relation() : sz(0), arty(0) {}
relation::relation(int n) : sz(0), arty(n) {}
relation::relation(const relation &r) : sz(r.sz), arty(r.arty), tuples(r.tuples) {}
	
relation::relation(int n, const vector<tuple> &ts) : sz(ts.size()), arty(n) {
	assert(arty > 0);
	vector<tuple>::const_iterator i;
	for (i = ts.begin(); i != ts.end(); ++i) {
		assert(i->size() == arty);
		tuple tail(i->begin() + 1, i->end());
		tuples[tail].insert(i->front());
	}
}
	
bool relation::has(const tuple &t) const {
	assert(t.size() == arty);
	tuple tail(t.begin() + 1, t.end());
	tuple_map::const_iterator i = tuples.find(tail);
	if (i == tuples.end()) {
		return false;
	}
	return in_set(i->second, t[0]);
}
	
void relation::slice(const tuple &inds, relation &out) const {
	assert(0 < inds.size() && inds.size() <= arty && inds[0] == 0);
	out.arty = inds.size();
	out.tuples.clear();
	tuple tinds;
	for (int i = 1; i < inds.size(); ++i) {
		tinds.push_back(inds[i] - 1);
	}
	
	out.sz = 0;
	tuple t;
	tuple_map::const_iterator i;
	for (i = tuples.begin(); i != tuples.end(); ++i) {
		slice_tuple(i->first, tinds, t);
		set<int> &s = out.tuples[t];
		out.sz -= s.size();
		s.insert(i->second.begin(), i->second.end());
		out.sz += s.size();
	}
}

bool relation::operator==(const relation &r) const {
	return arty == r.arty && sz == r.sz && tuples == r.tuples;
}

relation &relation::operator=(const relation &r) {
	sz = r.sz;
	arty = r.arty;
	tuples = r.tuples;
	return *this;
}

/*
 Remove all tuples in this relation that does not match any tuple in r
 along indexes inds.
*/
void relation::intersect(const tuple &inds, const relation &r) {
	assert(!inds.empty() && inds.front() == 0);
	tuple s;
	tuple tinds;
	for (int i = 1; i < inds.size(); ++i) {
		tinds.push_back(inds[i] - 1);
	}

	tuple_map::iterator i = tuples.begin();
	tuple_map::const_iterator j;
	while (i != tuples.end()) {
		slice_tuple(i->first, tinds, s);
		sz -= i->second.size();
		j = r.tuples.find(s);
		if (j == r.tuples.end()) {
			tuples.erase(i++);
		} else {
			intersect_sets_inplace(i->second, j->second);
			if (i->second.empty()) {
				tuples.erase(i++);
			} else {
				sz += i->second.size();
				++i;
			}
		}
	}
}

/*
 Remove all tuples in this relation that matches some tuple in r along indexes
 inds.
*/
void relation::subtract(const tuple &inds, const relation &r) {
	assert(!inds.empty() && inds.front() == 0);
	tuple s;
	tuple tinds;
	for (int i = 1; i < inds.size(); ++i) {
		tinds.push_back(inds[i] - 1);
	}

	tuple_map::iterator i = tuples.begin();
	tuple_map::const_iterator j;
	while (i != tuples.end()) {
		slice_tuple(i->first, tinds, s);
		j = r.tuples.find(s);
		if (j != r.tuples.end()) {
			sz -= i->second.size();
			subtract_sets_inplace(i->second, j->second);
			if (i->second.empty()) {
				tuples.erase(i++);
			} else {
				sz += i->second.size();
				++i;
			}
		} else {
			++i;
		}
	}
}

void relation::difference(const relation &r, relation &out) const {
	assert(arty == r.arty && arty == out.arty);
	tuple_map::const_iterator i, j;
	for (i = tuples.begin(); i != tuples.end(); ++i) {
		j = r.tuples.find(i->first);
		if (j != r.tuples.end()) {
			subtract_sets(i->second, j->second, out.tuples[i->first]);
		}
	}
	out.update_size();
}

/*
 For each tuple t1 in this relation, find all tuples t2 in r such that
 t1[match1] == t2[match2], and extend t1 with t2[extend]. Upon
 completion, this relation will contain all such t1's.
*/
void relation::expand(const relation  &r,
                      const tuple &match1,
                      const tuple &match2,
                      const tuple &extend)
{
	assert(!match1.empty() && match1.front() == 0 && !match2.empty() && match2.front() == 0);
	tuple m1, m2, ex;

	for (int i = 1; i < match1.size(); ++i) {
		m1.push_back(match1[i] - 1);
	}
	for (int i = 1; i < match2.size(); ++i) {
		m2.push_back(match2[i] - 1);
	}
	for (int i = 0; i < extend.size(); ++i) {
		ex.push_back(extend[i] - 1);
	}

	tuple_map old_tuples = tuples;
	tuples.clear();
	
	// preprocess r to avoid redundant slicing
	vector<sliced_relation_tuple> sliced(r.tuples.size());
	tuple_map::const_iterator i;
	int j = 0;
	for (i = r.tuples.begin(); i != r.tuples.end(); ++i) {
		slice_tuple(i->first, m2, sliced[j].match);
		slice_tuple(i->first, ex, sliced[j].extend);
		sliced[j++].lead = &i->second;
	}
	
	sz = 0;
	tuple t1, t2;
	for (i = old_tuples.begin(); i != old_tuples.end(); ++i) {
		slice_tuple(i->first, m1, t1);
		for (int j = 0; j < sliced.size(); ++j) {
			if (t1 == sliced[j].match) {
				t2 = concat_tuples(i->first, sliced[j].extend);
				set<int> &s = tuples[t2];
				sz -= s.size();
				intersect_sets(i->second, *sliced[j].lead, s);
				if (s.empty()) {
					tuples.erase(t2);
				}
				sz += s.size();
			}
		}
	}
	
	arty += extend.size();
}

void relation::count_expansion(const relation  &r,
                               const tuple &match1,
                               const tuple &match2,
							   int &matched,
							   int &new_size) const
{
	assert(!match1.empty() && match1.front() == 0 && !match2.empty() && match2.front() == 0);
	tuple m1, m2;
	tuple_map::const_iterator i;
	tuple t1, t2;

	for (int i = 1; i < match1.size(); ++i) {
		m1.push_back(match1[i] - 1);
	}
	for (int i = 1; i < match2.size(); ++i) {
		m2.push_back(match2[i] - 1);
	}
	// preprocess r to avoid redundant slicing
	vector<sliced_relation_tuple> sliced(r.tuples.size());
	int j = 0;
	for (i = r.tuples.begin(); i != r.tuples.end(); ++i) {
		slice_tuple(i->first, m2, sliced[j].match);
		sliced[j++].lead = &i->second;
	}

	matched = 0;
	new_size = 0;
	
	set<int> matched_insts;
	for (i = tuples.begin(); i != tuples.end(); ++i) {
		matched_insts.clear();
		slice_tuple(i->first, m1, t1);
		for (int j = 0; j < sliced.size(); ++j) {
			if (t1 == sliced[j].match) {
				new_size += intersect_sets(i->second, *sliced[j].lead, matched_insts);
			}
		}
		matched += matched_insts.size();
	}
}

void relation::add(const tuple &t) {
	assert(t.size() == arty);
	tuple tail(arty - 1);
	copy(t.begin() + 1, t.end(), tail.begin());
	add(t[0], tail);
}

void relation::add(int i, const tuple &t) {
	assert(t.size() + 1 == arty);
	set<int> &s = tuples[t];
	if (s.find(i) == s.end()) {
		s.insert(i);
		++sz;
	}
}

void relation::add(int i, int n) {
	assert(arty == 2);
	tuple t(1, n);
	add(i, t);
}

void relation::del(const tuple &t) {
	assert(t.size() == arty);
	tuple tail(arty - 1);
	copy(t.begin() + 1, t.end(), tail.begin());
	del(t[0], tail);
}

void relation::del(int i, const tuple &t) {
	assert(t.size() + 1 == arty);
	tuple_map::iterator j = tuples.find(t);
	if (j != tuples.end()) {
		set<int>::iterator k = j->second.find(i);
		if (k != j->second.end()) {
			j->second.erase(k);
			--sz;
		}
	}
}

void relation::del(int i, int n) {
	assert(arty == 2);
	tuple t(1, n);
	del(i, t);
}

void relation::at_pos(int n, set<int> &elems) const {
	assert(0 <= n && n < arty);
	tuple_map::const_iterator i;
	if (n == 0) {
		for (i = tuples.begin(); i != tuples.end(); ++i) {
			union_sets_inplace(elems, i->second);
		}
	} else {
		for (i = tuples.begin(); i != tuples.end(); ++i) {
			elems.insert(i->first[n - 1]);
		}
	}
}

void relation::drop_first(set<tuple> &out) const {
	tuple_map::const_iterator i;
	for (i = tuples.begin(); i != tuples.end(); ++i) {
		out.insert(i->first);
	}
}

void relation::clear() {
	sz = 0;
	tuples.clear();
}

void relation::reset(int new_arity) {
	tuples.clear();
	sz = 0;
	arty = new_arity;
}

/*
 Puts any tuple matching pattern pat into r. Any negate element in pat
 is considered a wild card. If pat is shorter than the relation's
 arity, the difference is considered to be wild cards.
*/
void relation::match(const tuple &pat, relation &r) const {
	assert(pat.size() <= arty && arty == r.arty);
	if (pat.empty()) {
		r.tuples = tuples;
		return;
	}
	
	tuple_map::const_iterator i;
	for (i = tuples.begin(); i != tuples.end(); ++i) {
		bool matched = true;
		for (int j = 1; j < pat.size(); ++j) {
			if (pat[j] >= 0 && pat[j] != i->first[j - 1]) {
				matched = false;
				break;
			}
		}
		if (matched) {
			set<int> &s = r.tuples[i->first];
			if (pat[0] < 0) {
				s = i->second;
			} else if (in_set(i->second, pat[0])) {
				s.insert(pat[0]);
			}
		}
	}
	r.update_size();
}

/*
 Keep only the tuples that match pattern pat.
*/
void relation::filter(const tuple &pat) {
	assert(pat.size() <= arty);
	if (pat.empty()) {
		return;
	}
	
	tuple_map::iterator i;
	for (i = tuples.begin(); i != tuples.end(); ++i) {
		bool matched = true;
		for (int j = 1; j < pat.size(); ++j) {
			if (pat[j] >= 0 && pat[j] != i->first[j - 1]) {
				matched = false;
				break;
			}
		}
		set<int> &s = i->second;
		if (!matched) {
			s.clear();
		} else if (pat[0] >= 0) {
			bool found = in_set(s, pat[0]);
			s.clear();
			if (found) {
				s.insert(pat[0]);
			}
		}
	}
	update_size();
}

void relation::update_size() {
	sz = 0;
	tuple_map::iterator i = tuples.begin();
	while (i != tuples.end()) {
		if (i->second.empty()) {
			tuples.erase(i++);
		} else {
			sz += i->second.size();
			++i;
		}
	}
}

void relation::sample(int k, relation &s) const {
	assert(k <= sz && s.arty == arty);
	
	if (k == sz) {
		s = *this;
		return;
	}
	vector<tuple> reservoir(k);
	tuple_map::const_iterator i;
	set<int>::const_iterator j;
	int n = 0;
	tuple t(arty);
	for (i = tuples.begin(); i != tuples.end(); ++i) {
		copy(i->first.begin(), i->first.end(), t.begin() + 1);
		for (j = i->second.begin(); j != i->second.end(); ++j) {
			t[0] = *j;
			if (n < k) {
				reservoir[n] = t;
			} else {
				int r = rand() % (n + 1);
				if (r < k) {
					reservoir[r] = t;
				}
			}
			++n;
		}
	}
	for (int ii = 0; ii < k; ++ii) {
		s.add(reservoir[ii]);
	}
}

void relation::serialize(std::ostream &os) const {
	serializer(os) << arty << sz << tuples;
}

void relation::unserialize(std::istream &is) {
	unserializer(is) >> arty >> sz >> tuples;
}

ostream &operator<<(ostream &os, const relation &r) {
	tuple t(r.arty);
	set<tuple> sorted;
	relation::tuple_map::const_iterator i;
	for (i = r.tuples.begin(); i != r.tuples.end(); ++i) {
		copy(i->first.begin(), i->first.end(), t.begin() + 1);
		set<int>::const_iterator j;
		for (j = i->second.begin(); j != i->second.end(); ++j) {
			t[0] = *j;
			sorted.insert(t);
		}
	}
	set<tuple>::iterator k;
	for (k = sorted.begin(); k != sorted.end(); ++k) {
		join(os, *k, " ") << endl;
	}
	return os;
}

ostream &operator<<(ostream &os, const relation_table &t) {
	relation_table::const_iterator i;
	for (i = t.begin(); i != t.end(); ++i) {
		os << i->first << endl << i->second;
	}
	return os;
}
