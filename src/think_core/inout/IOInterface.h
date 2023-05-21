#ifndef THINKSYSTEM_IOINTERFACE_H
#define THINKSYSTEM_IOINTERFACE_H
#include "../DefType.h"
#include "../NodeIdAllocator.h"
#include "../NodeInfo.h"

namespace think::io
{
struct IO_INFO
{
  size_t NumConstInputSize;
  size_t NumConstOutputSize;
};

class IOInterface
{
public:
  struct InterferenceFactorTable
  {
    WEIGHT<link_val> emotionActive       = 1.0;//������ԾȨ��
    WEIGHT<link_val> mentalConcentration = 1.0;//������Ȩ��
  };
  struct const_node_info
  {
    size_t in_size;
    size_t out_size;
  };

public:
  constexpr static const char *STR_CONST_ID_FOLDER = "./const_id/";
  static InterferenceFactorTable interference;//����ϵ����
protected:
  const ENodeType m_eIoType_;
  const const_node_info m_constNodeInfo_;

  std::vector<node_id> m_lstInputId_;
  std::vector<node_id> m_lstOutputId_;

  std::unique_ptr<LinkList> m_lpListInputLink_;

  //TODO ��Ҫ����һ�����������������ͣ����ڽ�����ֱ��תΪ��Ӧ���

public:
  explicit IOInterface(ENodeType nType, size_t inSize, size_t outSize)
  : m_eIoType_(nType),
    m_constNodeInfo_({inSize, outSize})
  {
  }
  virtual ~IOInterface() { saveConstId(); }
  virtual const std::unique_ptr<LinkList> &inputActInfo()                   = 0;
  virtual void loadActFunc(std::unordered_map<node_id, ActFunc> &OutputAct) = 0;

  bool saveConstId()
  {
    fs::path cIdPath = std::string(STR_CONST_ID_FOLDER) + ARRAY_TYPE_TO_STRING[(int) m_eIoType_];
    if (!fs::exists(cIdPath.parent_path())) { fs::create_directories(cIdPath.parent_path()); }
    std::ofstream idFile{cIdPath.string(), std::ios::binary | std::ios::out};

    if (!idFile.is_open())
    {
      std::cerr << "����δ֪���󣬾�̬�ڵ�id�ļ�д��ʱ��ʧ�ܣ�" << std::endl;
      return false;
    }
    const_node_info fileHeader{m_lstInputId_.size(), m_lstOutputId_.size()};
    idFile.write(reinterpret_cast<char *>(&fileHeader), sizeof(const_node_info));
    m_lstInputId_  = std::vector<node_id>(fileHeader.in_size);
    m_lstOutputId_ = std::vector<node_id>(fileHeader.out_size);

    idFile.write(reinterpret_cast<char *>(m_lstInputId_.data()),
                 static_cast<std::streamsize>(fileHeader.in_size * sizeof(node_id)));
    idFile.write(reinterpret_cast<char *>(m_lstOutputId_.data()),
                 static_cast<std::streamsize>(fileHeader.out_size * sizeof(node_id)));
    idFile.close();
  }

  void initConstId(NodeIdAllocator &IdAlloc, std::unordered_map<node_id, std::shared_ptr<NodeInfo>> &NodeBuffer)
  {
    std::string csPath = std::string(STR_CONST_ID_FOLDER) + ARRAY_TYPE_TO_STRING[(int) m_eIoType_];

    std::ifstream csFile{STR_CONST_ID_FOLDER, std::ios::binary | std::ios::in};
    if (!csFile.is_open())
    {
      if (!fs::exists(csPath))
      {
        std::cerr << "�ļ������ڣ���̬�ڵ�id�ļ���ȡʱ��ʧ�ܣ�" << std::endl;
        IdAlloc.allocate(m_lstInputId_, m_constNodeInfo_.in_size);
        IdAlloc.allocate(m_lstOutputId_, m_constNodeInfo_.out_size);
        std::cerr << "�ڵ��Ѿ����䡣" << std::endl;
        return ;
      }
      else std::cerr << "����δ֪���󣬾�̬�ڵ�id�ļ���ȡʱ��ʧ�ܣ�" << std::endl;

      return;
    }

    const_node_info fileHeader{};
    csFile.read(reinterpret_cast<char *>(&fileHeader), sizeof(const_node_info));
    m_lpListInputLink_ = std::make_unique<LinkList>(fileHeader.in_size);

    m_lstInputId_      = std::vector<node_id>(fileHeader.in_size);
    csFile.read(reinterpret_cast<char *>(m_lstInputId_.data()),
                static_cast<std::streamsize>(fileHeader.in_size * sizeof(node_id)));
    for (auto &[id, val]: (*m_lpListInputLink_))
    {
      csFile.read(reinterpret_cast<char *>(&id), sizeof(node_id));
      NodeBuffer.emplace(id, std::make_unique<NodeInfo>(id));
    }

    m_lstOutputId_ = std::vector<node_id>(fileHeader.out_size);
    csFile.read(reinterpret_cast<char *>(m_lstOutputId_.data()),
                static_cast<std::streamsize>(fileHeader.out_size * sizeof(node_id)));
    csFile.close();
  }

};//class IOInterface

IOInterface::InterferenceFactorTable IOInterface::interference = {};

}// namespace think::io
#endif// THINKSYSTEM_IOINTERFACE_H