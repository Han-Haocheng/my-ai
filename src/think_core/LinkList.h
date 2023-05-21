//
// Created by HanHaocheng on 2023/05/12.
//

#ifndef THINKSYSTEM_LINKLIST_H
#define THINKSYSTEM_LINKLIST_H
#include "DefType.h"

namespace think
{
/* �����б�����
 *  ��vector������������չ
 * �����������б���Ȩ��֮�������
 *  operator + - * /
 * ������ȥ��
 *  void deduplicate()
 * ���尴������ֵ�Ƴ�Ԫ�صķ���
 *  void erase(link_val standardValue)
 * */
class LinkList : public std::vector<link_t>
{
public:
  using data_type      = link_t;
  using super          = std::vector<data_type>;

  using iterator       = super::iterator;
  using const_iterator = super::const_iterator;
  using pointer        = super::pointer;
  using reference      = super::reference;

public:
  using std::vector<link_t>::vector;
  using std::vector<link_t>::operator=;

  LinkList &operator+=(int val);

  LinkList &operator-=(int val);

  LinkList &operator*=(const WEIGHT<link_val> &weight);

  LinkList operator*(const WEIGHT<link_val> &weight);

  LinkList operator-(int val);

  LinkList operator+(int val);

  void insert(LinkList &other) { std::vector<link_t>::insert(end(), other.begin(), other.end()); }
  void insert(LinkList &&other) { std::vector<link_t>::insert(end(), other.begin(), other.end()); }

  void weight(LinkList &other, WEIGHT<link_val> weight)
  {
    auto oldEnd = (std::ptrdiff_t) this->size();
    insert(other);
    std::for_each(this->begin() + oldEnd, this->end(),
                  [&](link_t &item) -> void { item.linkVal = weight * item.linkVal; });
  }

  void erase(link_val standardValue);

  /// @brief ȥ��
  /// ����ͬ��Ԫ�غϲ�������ֵ���
  void deduplicate();
};// class LinkList

void LinkList::deduplicate()
{
  std::unordered_set<link_t, LinkHash> s;
  s.reserve(this->size());
  for (link_t &lstLink: *this)
  {
    auto [setLink, bIsAdd] = s.emplace(lstLink);
    if (!bIsAdd) { const_cast<link_val &>(setLink->linkVal) += lstLink.linkVal; }
  }
  this->assign(s.begin(), s.end());
}
void LinkList::erase(link_val standardValue)
{
  if (this->empty()) { return; }
  auto condition = [&](const link_t &item) -> bool { return item.linkVal < standardValue; };
  auto res       = std::remove_if(this->begin(), this->end(), condition);
  super::erase(res);
}

LinkList LinkList::operator+(int val)
{
  LinkList res = *this;
  return res += val;
}
LinkList LinkList::operator-(int val)
{
  LinkList res = *this;
  return res -= val;
}
LinkList LinkList::operator*(const WEIGHT<link_val> &weight)
{
  LinkList res = *this;
  return res *= weight;
}
LinkList &LinkList::operator*=(const WEIGHT<link_val> &weight)
{
  for (link_t &lk: *this) { lk.linkVal = weight * lk.linkVal; }
  return *this;
}
LinkList &LinkList::operator-=(int val)
{
  for (link_t &lk: *this) { lk.linkVal = std::max<int>(NUM_LINK_VAL_MIN, lk.linkVal - val); }
  return *this;
}
LinkList &LinkList::operator+=(int val)
{
  for (link_t &lk: *this) { lk.linkVal = std::min<int>(NUM_LINK_VAL_MAX, lk.linkVal + val); }
  return *this;
}

}// namespace think

#endif//THINKSYSTEM_LINKLIST_H
