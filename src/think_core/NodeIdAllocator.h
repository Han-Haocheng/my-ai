//
// Created by HanHaocheng on 2023/05/12.
//

#ifndef THINKSYSTEM_NODEIDALLOCATOR_H
#define THINKSYSTEM_NODEIDALLOCATOR_H
#include "DefType.h"
#include "base_type/range.h"

namespace think
{

///\brief �ڵ�id������
class NodeIdAllocator
{
private:
  using IdRange = Range<node_id>;
  std::forward_list<IdRange> m_cListUnassignedIDs;

  constexpr static const char *STR_UNASSIGNED_PATH = "id_allocator.dat";

public:
  explicit NodeIdAllocator(size_t nodeCount);

  ~NodeIdAllocator();

  /// ����ڵ㣨ʱ�临�Ӷ�O(1)��
  /// \param out
  node_id allocate();

  /// ����ڵ㣨ʱ�临�Ӷ�O(n)��
  /// \param out
  /// \param count
  void allocate(std::vector<node_id> &out, size_t count);

  /// �ͷŽڵ�id��ʱ�临�Ӷ� O(n)��
  /// \param in
  void release(const node_id &in);

  /// �ͷŽڵ㣨ʱ�临�Ӷ� O(n^2)��
  /// \param in
  void release(const std::vector<node_id> &in);

private:
  void _getUnassignedId(const std::string &path);
  void _saveUnassignedId(const std::string &path);
};// class NodeIdAllocator

NodeIdAllocator::NodeIdAllocator(size_t nodeCount)
{
  if (fs::exists(STR_UNASSIGNED_PATH)) { _getUnassignedId(STR_UNASSIGNED_PATH); }
  else { m_cListUnassignedIDs.emplace_front(node_id{1}, node_id{nodeCount}); }
}
NodeIdAllocator::~NodeIdAllocator() { _saveUnassignedId(STR_UNASSIGNED_PATH); }
node_id NodeIdAllocator::allocate()
{
  node_id out = m_cListUnassignedIDs.front().begin()++;
  if (m_cListUnassignedIDs.front().is_empty()) { m_cListUnassignedIDs.pop_front(); }
  return out;
}

void NodeIdAllocator::allocate(std::vector<node_id> &out, size_t count)
{
  if (count == 0) { return; }
  out.reserve(count);
  auto beg = m_cListUnassignedIDs.begin(), bfBeg = m_cListUnassignedIDs.before_begin();
  size_t i = 0;
  while (beg != m_cListUnassignedIDs.end())
  {
    for (; beg->begin() < beg->end(); ++beg->begin())
    {
      out.emplace_back(beg->begin()++);
      if (--count == 0) { return; }
    }
    m_cListUnassignedIDs.erase_after(bfBeg);
    beg = ++bfBeg;
  }
  throw std::overflow_error("Error:���нڵ�����Ϊ�գ�������ڵ㣡");
}

void NodeIdAllocator::release(const node_id &in)
{
  //�ж��ͷŽڵ��Ƿ�����ǰ��
  auto itBeg = m_cListUnassignedIDs.begin();
  IdRange installRange(in, in + 1);
  if (in < itBeg->begin())
  {
    if (installRange.end() == itBeg->begin()) { --itBeg->begin(); }
    else { m_cListUnassignedIDs.emplace_front(installRange); }
  }
  else
  {
    auto itBfBeg = m_cListUnassignedIDs.before_begin();

    //���ҵ�һ��begin_����in��λ��
    for (; itBeg != m_cListUnassignedIDs.end(); ++itBeg, ++itBfBeg)
    {
      if (itBeg->begin() > in)
      {
        //in����һ����Χ��
        ////�׳��쳣
        //in����һ����Χ�����������Χ��ʼ��λ��
        ////
        if (itBfBeg->contains(in)) { throw std::runtime_error("Error:�ڵ��ͷ��쳣��"); }
        if (itBfBeg->end() == installRange.begin())
        {
          if (installRange.end() == itBeg->begin())
          {
            itBfBeg->end() = itBeg->end();
            m_cListUnassignedIDs.erase_after(itBfBeg);
          }
          else { itBfBeg->end() = installRange.end(); }
        }
        else if (installRange.end() == itBeg->begin()) { itBeg->begin() = installRange.begin(); }
        else { auto newIter = m_cListUnassignedIDs.insert_after(itBfBeg, installRange); }
      }
    }

    //�����ͷ�λ��δ�ҵ���˵���ͷ�λ��Ϊ���һ����Χ��
    if (itBfBeg->end() == in) { ++itBfBeg->end(); }
    else { m_cListUnassignedIDs.insert_after(itBfBeg, {in, in + 1}); }
  }
}
void NodeIdAllocator::release(const std::vector<node_id> &in)
{
  if (in.empty()) { return; }
  for (const auto item: in) { release(item); }
}
void NodeIdAllocator::_getUnassignedId(const std::string &path)
{
  std::ifstream file;
  IdRange tmp;
  file.open(path, std::ios::binary | std::ios::in);
  if (file.is_open())
  {
    while (file.read(reinterpret_cast<char *>(&tmp), sizeof(IdRange))) { m_cListUnassignedIDs.emplace_front(tmp); }
    file.close();
  }
}
void NodeIdAllocator::_saveUnassignedId(const std::string &path)
{
  std::ofstream file(path, std::ios::binary | std::ios::out);
  if (file.is_open())
  {
    for (const auto &id: m_cListUnassignedIDs) { file.write(reinterpret_cast<const char *>(&id), sizeof(IdRange)); }
    file.close();
  }
}
//! class NodeIDAllocator

}// namespace think

#endif//THINKSYSTEM_NODEIDALLOCATOR_H
