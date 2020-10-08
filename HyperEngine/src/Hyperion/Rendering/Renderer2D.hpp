#pragma once

#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Core/Core.hpp"
#include "Buffers/Vertex.hpp"
#include "Buffers/IndexBuffer.hpp"
#include "Buffers/VertexArray.hpp"
#include "Buffers/VertexBuffer.hpp"
#include "Maths/Matrix.hpp"
#include "Maths/Vector.hpp"

namespace Hyperion
{
	class Renderer2D
	{
	private:
		static ShaderManager* m_ShaderManager;
		static TextureManager* m_TextureManager;

		static const uint32_t MaxQuads;
		static const uint32_t MaxVertices;
		static const uint32_t MaxIndices;

		static Vector4<float> m_QuadVertexPositions[4];

		static Ref<VertexArray> m_QuadVertexArray;
		static Ref<VertexBuffer> m_QuadVertexBuffer;
		static Ref<IndexBuffer> m_QuadIndexBuffer;
		static uint32_t m_QuadShader;

		static Vertex2D* m_QuadVertexBufferBase;
		static Vertex2D* m_QuadVertexBufferPtr;

		static uint32_t m_QuadCount;
		static uint32_t m_QuadIndexCount;
		static uint32_t m_DrawCalls;

	public:
		static void Init();

		static void BeginScene(uint32_t width, uint32_t height, float zoom, float nearPlane, float farPlane, const Vector3<float>& position);
		static void EndScene();

		static void Flush();
		static void FlushAndReset();

		static void DrawQuad(const Vector3<float>& position, const Vector3<float>& rotation, const Vector3<float>& scale, const Vector4<float>& color);

		static void SetShaderManager(ShaderManager* shaderManager);
		static void SetTextureManager(TextureManager* textureManager);
	};
}
