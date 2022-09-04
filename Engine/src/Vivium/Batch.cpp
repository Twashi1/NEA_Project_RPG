#include "Batch.h"

namespace Vivium {
	Batch::Batch(const std::size_t& max_count, const BufferLayout* layout)
		: m_MaxCount(max_count), m_Layout(layout)
	{
		std::size_t vertex_stride = layout->GetStride();
		m_Vertices = new float[max_count * vertex_stride];
		m_Indices = new uint16_t[max_count * 6]; // NOTE: Assuming 6 indices per shape
	}

	void Batch::Submit(const Vector2<float>& pos, const Vector2<float>& size)
	{
		float halfwidth = size.x * 0.5f;
		float halfheight = size.y * 0.5f;

		float left = pos.x - halfwidth;
		float right = pos.x + halfwidth;
		float bottom = pos.y - halfheight;
		float top = pos.y + halfheight;

		// Bottom left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = bottom;
		// Bottom right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = bottom;
		// Top right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = top;
		// Top left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = top;

		std::size_t indices_offset = m_Count * 4; // 4 vertices per shape
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;
		m_Indices[m_IndicesIndex++] = 1 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 3 + indices_offset;
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;
		
		// Increment shape count
		++m_Count;
	}

	void Batch::Submit(const Vector2<float>& pos, const Vector2<float>& size, const float tex_coords[8])
	{
		float halfwidth = size.x * 0.5f;
		float halfheight = size.y * 0.5f;

		float left = pos.x - halfwidth;
		float right = pos.x + halfwidth;
		float bottom = pos.y - halfheight;
		float top = pos.y + halfheight;

		// Bottom left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = bottom;
		m_Vertices[m_VerticesIndex++] = tex_coords[0];
		m_Vertices[m_VerticesIndex++] = tex_coords[1];
		// Bottom right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = bottom;
		m_Vertices[m_VerticesIndex++] = tex_coords[2];
		m_Vertices[m_VerticesIndex++] = tex_coords[3];
		// Top right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = top;
		m_Vertices[m_VerticesIndex++] = tex_coords[4];
		m_Vertices[m_VerticesIndex++] = tex_coords[5];
		// Top left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = top;
		m_Vertices[m_VerticesIndex++] = tex_coords[6];
		m_Vertices[m_VerticesIndex++] = tex_coords[7];

		std::size_t indices_offset = m_Count * 4; // 4 vertices per shape
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;
		m_Indices[m_IndicesIndex++] = 1 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 3 + indices_offset;
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;

		// Increment shape count
		++m_Count;
	}

	void Batch::Submit(const Vector2<float>& pos, const Vector2<float>& size, float tex_left, float tex_right, float tex_bottom, float tex_top)
	{
		float halfwidth = size.x * 0.5f;
		float halfheight = size.y * 0.5f;

		float left = pos.x - halfwidth;
		float right = pos.x + halfwidth;
		float bottom = pos.y - halfheight;
		float top = pos.y + halfheight;

		// Bottom left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = bottom;
		m_Vertices[m_VerticesIndex++] = tex_left;
		m_Vertices[m_VerticesIndex++] = tex_bottom;
		// Bottom right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = bottom;
		m_Vertices[m_VerticesIndex++] = tex_right;
		m_Vertices[m_VerticesIndex++] = tex_bottom;
		// Top right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = top;
		m_Vertices[m_VerticesIndex++] = tex_right;
		m_Vertices[m_VerticesIndex++] = tex_top;
		// Top left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = top;
		m_Vertices[m_VerticesIndex++] = tex_left;
		m_Vertices[m_VerticesIndex++] = tex_top;

		std::size_t indices_offset = m_Count * 4; // 4 vertices per shape
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;
		m_Indices[m_IndicesIndex++] = 1 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 3 + indices_offset;
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;

		// Increment shape count
		++m_Count;
	}

	void Batch::Submit(float left, float right, float bottom, float top, float tex_left, float tex_right, float tex_bottom, float tex_top)
	{
		// Bottom left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = bottom;
		m_Vertices[m_VerticesIndex++] = tex_left;
		m_Vertices[m_VerticesIndex++] = tex_bottom;
		// Bottom right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = bottom;
		m_Vertices[m_VerticesIndex++] = tex_right;
		m_Vertices[m_VerticesIndex++] = tex_bottom;
		// Top right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = top;
		m_Vertices[m_VerticesIndex++] = tex_right;
		m_Vertices[m_VerticesIndex++] = tex_top;
		// Top left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = top;
		m_Vertices[m_VerticesIndex++] = tex_left;
		m_Vertices[m_VerticesIndex++] = tex_top;

		std::size_t indices_offset = m_Count * 4; // 4 vertices per shape
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;
		m_Indices[m_IndicesIndex++] = 1 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 3 + indices_offset;
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;

		// Increment shape count
		++m_Count;
	}

	void Batch::Submit(float* data, std::size_t count)
	{
		std::memcpy(m_Vertices + m_VerticesIndex * sizeof(float), data, count * sizeof(float));

		m_VerticesIndex += count;

		std::size_t indices_offset = m_Count * 4; // 4 vertices per shape
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;
		m_Indices[m_IndicesIndex++] = 1 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 3 + indices_offset;
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;

		++m_Count;
	}

	void Batch::Submit(const Vector2<float>& pos, const Vector2<float>& size, float tex_left, float tex_right, float tex_bottom, float tex_top, float* vertex_data, unsigned int vertex_data_count)
	{
		float halfwidth = size.x * 0.5f;
		float halfheight = size.y * 0.5f;

		float left = pos.x - halfwidth;
		float right = pos.x + halfwidth;
		float bottom = pos.y - halfheight;
		float top = pos.y + halfheight;

		// Bottom left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = bottom;
		m_Vertices[m_VerticesIndex++] = tex_left;
		m_Vertices[m_VerticesIndex++] = tex_bottom;

		for (unsigned int i = 0; i < vertex_data_count; i++) {
			m_Vertices[m_VerticesIndex++] = vertex_data[i];
		}

		// Bottom right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = bottom;
		m_Vertices[m_VerticesIndex++] = tex_right;
		m_Vertices[m_VerticesIndex++] = tex_bottom;

		for (unsigned int i = 0; i < vertex_data_count; i++) {
			m_Vertices[m_VerticesIndex++] = vertex_data[i];
		}

		// Top right
		m_Vertices[m_VerticesIndex++] = right;
		m_Vertices[m_VerticesIndex++] = top;
		m_Vertices[m_VerticesIndex++] = tex_right;
		m_Vertices[m_VerticesIndex++] = tex_top;

		for (unsigned int i = 0; i < vertex_data_count; i++) {
			m_Vertices[m_VerticesIndex++] = vertex_data[i];
		}

		// Top left
		m_Vertices[m_VerticesIndex++] = left;
		m_Vertices[m_VerticesIndex++] = top;
		m_Vertices[m_VerticesIndex++] = tex_left;
		m_Vertices[m_VerticesIndex++] = tex_top;

		for (unsigned int i = 0; i < vertex_data_count; i++) {
			m_Vertices[m_VerticesIndex++] = vertex_data[i];
		}

		std::size_t indices_offset = m_Count * 4; // 4 vertices per shape
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;
		m_Indices[m_IndicesIndex++] = 1 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 2 + indices_offset;
		m_Indices[m_IndicesIndex++] = 3 + indices_offset;
		m_Indices[m_IndicesIndex++] = 0 + indices_offset;

		// Increment shape count
		++m_Count;
	}

	Batch::BatchData Batch::End() const
	{
		BatchData data;
		data.vertex_buffer = MakeRef(VertexBuffer, m_Vertices, m_VerticesIndex + 1, *m_Layout);
		data.index_buffer = MakeRef(IndexBuffer, m_Indices, m_IndicesIndex + 1);
		data.count = m_Count;

		return data;
	}

	Batch::~Batch()
	{
		delete[] m_Vertices;
		delete[] m_Indices;
	}

	Batch::BatchData::BatchData()
		: vertex_buffer(nullptr), index_buffer(nullptr), count(0)
	{}

	Batch::BatchData::BatchData(const BatchData& other)
		: vertex_buffer(other.vertex_buffer), index_buffer(other.index_buffer), count(other.count)
	{}

	Batch::BatchData::BatchData(BatchData&& other) noexcept
		: vertex_buffer(std::move(other.vertex_buffer)), index_buffer(std::move(other.index_buffer)), count(std::move(other.count))
	{}
}