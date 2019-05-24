#include "stdafx.h"
#include "Const.h"
#include "GraphElement.h"
#include "Algorithm.h"
#include "AlgoLineSweep.h"


void GetLeftInSets(const RealPoint& p, SegBST& tree, SegVec& rightSet, SegVec& inSet)
{
	if( tree.empty() )
		return;

	SwpSegment s(p, p);
	SegBSTIterator it = tree.upper_bound(&s);
	SegBSTIterator tmpBSTIt;
	it = tree.upper_bound(&s);
	reverse_iterator<SegBSTIterator> rit(it);

	while( rit != tree.rend() && fabs((*rit)->Height(p) - p.y) < EPS )
	{
		if( (*rit)->IsOnSegment(p) )
			inSet.push_back(*rit);
		else if( (*rit)->IsRightEnd(p) )
			rightSet.push_back(*rit);
		rit ++;
	}
}

void SegVecUnion(SegVecIterator& begin, SegVecIterator& last, SegVecIterator& result)
{
	SegVecIterator it;
	for( it = begin ; it!=last; it++,result++ )
	{
		*result = *it;
	}
}

void FindNeighbors(const RealPoint &p, SegBST &btree, SwpSegment *&above, SwpSegment *&below)
{
	SwpSegment s(p, p);
	SegBSTIterator it = btree.upper_bound(&s);

	if( it == btree.end() )
		above = NULL;
	else
		above = *it;

	if( it == btree.begin() )
		below = NULL;
	else
		below = *--it;
}

bool ComputeNewEvents(SwpSegment *s0, SwpSegment *s1, const Event &p, EventQueue &q, RealPoint &inter)
{
	if( s0 != NULL && s1 != NULL && s1->Intersect(*s0, inter) )
	{
		Event evt(ET_Inter, inter);
		if( p < evt && !q.IsExist(evt) )
		{
			q.Push(evt);
			return true;
		}
	}
	return false;
}

void FindEndSeg(SegVec& v, const RealPoint& p, SwpSegment*&min, SwpSegment*& max)
{
	SegVecIterator it = v.begin();
	min = *v.begin();
	max = min;
	it ++;
	while( it != v.end() )
	{
		if( (*it)->Less(*min,p) )
			min = *it;
		if( max->Less(**it,p) )
			max = *it;
		it++;
	}
}

SwpSegment* FindUpperNeighbor(SwpSegment* s, SegBST& btree)
{
	SegBSTIterator it = btree.find(s);
	++it;
	if( it==btree.end())
		return NULL;
	else
		return *it;
}

SwpSegment* FindLowerNeighbor(SwpSegment* s, SegBST& btree)
{
	SegBSTIterator it = btree.find(s);
	if( it==btree.begin() )
		return NULL;
	else
		return *(--it);
}
 
bool GetIntersection(vector<RealSegment> &sgs, vector<RealPoint> &pts)
{
	pts.clear();

	vector<SwpSegment> tsg;
	for(UINT i = 0; i < sgs.size(); i++)
	{
		tsg.push_back(SwpSegment(sgs[i].m_s, sgs[i].m_e));
	}

	EventQueue evtQueue;
	for(UINT i = 0; i < tsg.size(); i++)
	{
		evtQueue.Push(Event(ET_Start, tsg[i].m_s), (SwpSegment*)(&tsg[i]));
		evtQueue.Push(Event(ET_End, tsg[i].m_e));
	}

	//初始化存储线段的平衡二叉树
	RealPoint swp(0,0);
	sCompare compare(&swp);
	SegBST segBSTree(compare);
	int count = 0;

	while(!evtQueue.empty())
	{
		Event evt		= evtQueue.TopEvent();
		SegVec leftSet	= evtQueue.TopSegVec();
		evtQueue.Pop();

		RealPoint tmpPoint = swp;
		RealPoint evtPoint	= evt.m_pt;

		//更新扫描线
		swp = evtPoint;

		SegVec rightSet, inSet;
		GetLeftInSets(evtPoint, segBSTree, rightSet, inSet);

		int lSize = leftSet.size();
		int rSize = rightSet.size();
		int iSize = inSet.size();
		if(lSize + rSize + iSize > 1)
		{
			pts.push_back(evtPoint);
			TRACE(_T("%8.2f, %8.2f\n"), evtPoint.x, evtPoint.y);
		}

		SegBSTIterator tmpBSTIt;
		SegVecIterator tmpSegIt;

		swp = tmpPoint;
		//移除rightSet和inSet
		for(SegVecIterator segIt = rightSet.begin(); segIt!=rightSet.end(); segIt++)
		{
			segBSTree.erase(*segIt);
		}
		for(SegVecIterator segIt = inSet.begin(); segIt!=inSet.end(); segIt++)
		{
			segBSTree.erase(*segIt);
		}

		swp = evtPoint;
		//插入leftSet和inSet中的线段集
		for(SegVecIterator segIt = leftSet.begin(); segIt!=leftSet.end(); segIt++)
		{
			segBSTree.insert(*segIt);
		}
		for(SegVecIterator segIt = inSet.begin(); segIt!=inSet.end(); segIt++)
		{
			segBSTree.insert(*segIt);
		}

		if( (lSize + iSize) == 0)
		{
			SwpSegment* sa,*sb;
			FindNeighbors(evtPoint, segBSTree, sa, sb);
			RealPoint inter;
			if (ComputeNewEvents(sa, sb, evt, evtQueue, inter))
			{
				pts.push_back(inter);
			}
		}
		else
		{
			SegVec v(lSize + iSize);
			SegVecUnion(leftSet.begin(), leftSet.end(), v.begin());
			SegVecUnion(inSet.begin(), inSet.end(), v.begin());

			SwpSegment* bot, *top;
			FindEndSeg(v, evtPoint, bot, top);

			SwpSegment* lowerBot = FindLowerNeighbor(bot, segBSTree);
			SwpSegment* upperTop = FindUpperNeighbor(top, segBSTree);
			RealPoint inter;
			if (ComputeNewEvents(bot, lowerBot, evt, evtQueue, inter))
			{
				pts.push_back(inter);
			}
			
			if (ComputeNewEvents(top, upperTop, evt, evtQueue, inter))
			{
				pts.push_back(inter);
			}
		}
	}
	return true;
}