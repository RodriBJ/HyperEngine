#include "OpenGL33Renderer2D.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "HyperMath/Quaternion.hpp"

namespace Hyperion
{
	OpenGL33Renderer2D::OpenGL33Renderer2D()
	{
		m_QuadVertexArray = CreateRef<OpenGL33VertexArray>(VertexLayout::Vertex2D);
		m_QuadVertexArray->Bind();

		m_QuadVertexBuffer = CreateRef<OpenGL33VertexBuffer>(VertexLayout::Vertex2D, MaxVertices);
		m_QuadVertexBuffer->Bind();

		m_QuadVertexBufferBase = new Vertex2D[MaxVertices];

		uint32_t* quadIndices = new uint32_t[MaxIndices];
		uint32_t offset = 0;

		for (uint32_t i = 0; i < MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		m_QuadIndexBuffer = CreateRef<OpenGL33IndexBuffer>(quadIndices, MaxIndices);
		m_QuadIndexBuffer->Bind();
		delete[] quadIndices;

		m_QuadVertexPositions[0] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_QuadVertexPositions[2] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		m_QuadVertexArray->Init();
	}

	OpenGL33Renderer2D::~OpenGL33Renderer2D()
	{
	}

	void OpenGL33Renderer2D::BeginScene(uint32_t width, uint32_t height, float zoom, float nearPlane, float farPlane, const Vec3& position)
	{
		float aspectRatio = (float) width / height;

		m_ShaderManager->UseShader(0);
		m_ShaderManager->SetMatrix4(0, "u_ProjectionMatrix", Matrix::Ortho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom, nearPlane, farPlane));
		m_ShaderManager->SetMatrix4(0, "u_TransformationMatrix", Matrix::Translate(Mat4(1.0f), Vector::Inverse(position)));

		m_QuadCount = 0;
		m_QuadIndexCount = 0;
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
	}

	void OpenGL33Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)m_QuadVertexBufferPtr - (uint8_t*)m_QuadVertexBufferBase);
		m_QuadVertexBuffer->Bind();
		m_QuadVertexBuffer->SetData(m_QuadVertexBufferBase, dataSize);

		Flush();
	}

	void OpenGL33Renderer2D::Flush()
	{
		if (m_QuadIndexCount == 0)
			return;

		m_ShaderManager->UseShader(0);

		m_QuadVertexArray->Bind();
		glDrawElements(GL_TRIANGLES, (unsigned int) m_QuadIndexCount, GL_UNSIGNED_INT, nullptr);

		m_DrawCalls++;
	}

	void OpenGL33Renderer2D::FlushAndReset()
	{
		EndScene();

		m_QuadCount = 0;
		m_QuadIndexCount = 0;
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
	}

	void OpenGL33Renderer2D::DrawQuad(const Vec3& position, const Vec3& rotation, const Vec3& scale, const Vec4& color)
	{
		constexpr size_t quadVertexCount = 4;
		const float tilingFactor = 1.0f;

		if (m_QuadIndexCount >= MaxIndices)
			FlushAndReset();

		Mat4 translateMatrix = Mat4(1.0f);
		translateMatrix = Matrix::Translate(translateMatrix, position);

		Mat4 scaleMatrix = Mat4(1.0f);
		scaleMatrix = Matrix::Scale(scaleMatrix, scale);

		Vec4 relativeTranslation = Vec4(0.5f * scale.x, 0.5f * scale.y, 0.5f * scale.z, 1.0f);

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Vec4 translatedVector = scaleMatrix * m_QuadVertexPositions[i];

			Mat4 rotationMatrix = Quaternion::ConvertToMatrix(Quaternion(rotation));
			translatedVector = rotationMatrix * translatedVector;

			translatedVector = translateMatrix * translatedVector;

			m_QuadVertexBufferPtr->Position = Vec3(translatedVector.x, translatedVector.y, translatedVector.z);
			m_QuadVertexBufferPtr->Color = color;
			m_QuadVertexBufferPtr->TextureCoords = {};
			m_QuadVertexBufferPtr->TextureId = -1;
			m_QuadVertexBufferPtr++;
		}
		
		m_QuadIndexCount += 6;
		m_QuadCount++;
	}

	void OpenGL33Renderer2D::SetShaderManager(Ref<ShaderManager> shaderManager)
	{
		m_ShaderManager = shaderManager;
	}

	void OpenGL33Renderer2D::SetTextureManager(Ref<TextureManager> textureManager)
	{
		m_TextureManager = textureManager;
	}
}
