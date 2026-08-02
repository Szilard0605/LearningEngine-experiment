#include "Mesh.h"

#include "Log/Log.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material)
	: m_Vertices(vertices), m_Indices(indices)
{
	LE_CORE_ASSERT(vertices.size() > 0, "Mesh: vertices size is 0");
	
	LE_CORE_INFO("[Mesh] Creating vertex buffer with %d vertices", vertices.size());

	m_VertexBuffer = VertexBuffer::Create(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(Vertex)));
	
	m_VertexBuffer->SetLayout({
		{ ShaderDataType::Float3, "a_position"  }, 
		{ ShaderDataType::Float3, "a_normal"    },
		{ ShaderDataType::Float3, "a_tangent"   },
		{ ShaderDataType::Float3, "a_bitangent" },
		{ ShaderDataType::Float2, "a_texcoords" },
		{ ShaderDataType::Int,    "a_entity    "}
	});

	m_VertexArray = VertexArray::Create();
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	m_IndexBuffer = IndexBuffer::Create(indices.data(), static_cast<uint32_t>(indices.size()));
	m_Material = new Material(material);
}

Mesh::~Mesh()
{

}

void Mesh::Render()
{
	m_VertexArray->Bind();
	m_IndexBuffer->Bind();
	m_VertexArray->DrawIndexed(m_IndexBuffer->GetCount());
}

