#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/GraphicsBuffer.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/Renderable.hpp>
#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/Texture.hpp>
#include <KlayGE/RenderSettings.hpp>
#include <KlayGE/Sampler.hpp>
#include <KlayGE/PerlinNoise.hpp>

#include <KlayGE/D3D9/D3D9RenderFactory.hpp>

#include <vector>
#include <sstream>
#include <ctime>

#include "Electro.hpp"

using namespace std;
using namespace KlayGE;

namespace
{
	class RenderElectro : public RenderableHelper
	{
	public:
		RenderElectro()
			: RenderableHelper(L"Electro")
		{
			RenderFactory& rf = Context::Instance().RenderFactoryInstance();

			MathLib::PerlinNoise<float>& pn = MathLib::PerlinNoise<float>::Instance();

			int const XSIZE = 128;
			int const YSIZE = 32;
			int const ZSIZE = 32;
			float const XSCALE = 0.04f;
			float const YSCALE = 0.08f;
			float const ZSCALE = 0.08f;

			std::vector<unsigned char> turbBuffer;
			turbBuffer.reserve(XSIZE * YSIZE * ZSIZE);
			unsigned int min = 255, max = 0;
			for (int z = 0; z < ZSIZE; ++ z)
			{
				for (int y = 0; y < YSIZE; ++ y)
				{
					for (int x = 0; x < XSIZE; ++ x)
					{
						unsigned char t = static_cast<unsigned char>(127 * (1
							+ pn.tileable_turbulence(XSCALE * x, YSCALE * y, ZSCALE * z,
								XSIZE * XSCALE, YSIZE * YSCALE, ZSIZE * ZSCALE, 16)));
						if (t > max)
						{
							max = t;
						}
						if (t < min)
						{
							min = t;
						}

						turbBuffer.push_back(t);
					}
				}
			}
			for (unsigned int i = 0; i < XSIZE * YSIZE * ZSIZE; ++ i)
			{
				turbBuffer[i] = (255 * (turbBuffer[i] - min)) / (max - min);
			}

			TexturePtr texture = rf.MakeTexture3D(XSIZE, YSIZE, ZSIZE, 1, PF_L8);
			texture->CopyMemoryToTexture3D(0, &turbBuffer[0], PF_L8, XSIZE, YSIZE, ZSIZE, 0, 0, 0,
				XSIZE, YSIZE, ZSIZE);

			effect_ = rf.LoadEffect("Electro.fx");
			effect_->ActiveTechnique("Electro");

			SamplerPtr electro_sampler(new Sampler);
			electro_sampler->SetTexture(texture);
			electro_sampler->Filtering(Sampler::TFO_Bilinear);
			electro_sampler->AddressingMode(Sampler::TAT_Addr_U, Sampler::TAM_Wrap);
			electro_sampler->AddressingMode(Sampler::TAT_Addr_V, Sampler::TAM_Wrap);
			electro_sampler->AddressingMode(Sampler::TAT_Addr_W, Sampler::TAM_Wrap);
			*(effect_->ParameterByName("electroSampler")) = electro_sampler;

			Vector3 xyzs[] =
			{
				Vector3(-0.8f, 0.8f, 1),
				Vector3(0.8f, 0.8f, 1),
				Vector3(-0.8f, -0.8f, 1),
				Vector3(0.8f, -0.8f, 1),
			};

			Vector3 texs[] =
			{
				Vector3(-1, 0, 0),
				Vector3(1, 0, 0),
				Vector3(-1, -1, 0),
				Vector3(1, -1, 0),
			};

			uint16_t indices[] = 
			{
				0, 1, 3, 3, 2, 0,
			};

			rl_ = rf.MakeRenderLayout(RenderLayout::BT_TriangleList);

			GraphicsBufferPtr pos_vb = rf.MakeVertexBuffer(BU_Static);
			pos_vb->Resize(sizeof(xyzs));
			{
				GraphicsBuffer::Mapper mapper(*pos_vb, BA_Write_Only);
				std::copy(&xyzs[0], &xyzs[0] + sizeof(xyzs) / sizeof(xyzs[0]), mapper.Pointer<Vector3>());
			}
			GraphicsBufferPtr tex0_vb = rf.MakeVertexBuffer(BU_Static);
			tex0_vb->Resize(sizeof(texs));
			{
				GraphicsBuffer::Mapper mapper(*tex0_vb, BA_Write_Only);
				std::copy(&texs[0], &texs[0] + sizeof(texs) / sizeof(texs[0]), mapper.Pointer<Vector3>());
			}

			rl_->BindVertexStream(pos_vb, boost::make_tuple(vertex_element(VEU_Position, 0, sizeof(float), 3)));
			rl_->BindVertexStream(tex0_vb, boost::make_tuple(vertex_element(VEU_TextureCoord, 0, sizeof(float), 3)));

			GraphicsBufferPtr ib = rf.MakeIndexBuffer(BU_Static);
			ib->Resize(sizeof(indices));
			{
				GraphicsBuffer::Mapper mapper(*ib, BA_Write_Only);
				std::copy(indices, indices + sizeof(indices) / sizeof(uint16_t), mapper.Pointer<uint16_t>());
			}
			rl_->BindIndexStream(ib, IF_Index16);

			box_ = MathLib::ComputeBoundingBox<float>(&xyzs[0], &xyzs[0] + sizeof(xyzs) / sizeof(xyzs[0]));
		}

		void OnRenderBegin()
		{
			float const t = std::clock() * 0.0002f;

			*(effect_->ParameterByName("y")) = t * 2;
			*(effect_->ParameterByName("z")) = t;
		}
	};

	bool ConfirmDevice(RenderDeviceCaps const & caps)
	{
		if (caps.max_shader_model < 2)
		{
			return false;
		}
		return true;
	}
}

int main()
{
	Context::Instance().RenderFactoryInstance(D3D9RenderFactoryInstance());

	RenderSettings settings;
	settings.width = 800;
	settings.height = 600;
	settings.colorDepth = 32;
	settings.fullScreen = false;
	settings.ConfirmDevice = ConfirmDevice;

	Electro app;
	app.Create("Electro", settings);
	app.Run();

	return 0;
}

Electro::Electro()
{
	ResLoader::Instance().AddPath("../media/Common");
	ResLoader::Instance().AddPath("../media/Electro");
}

void Electro::InitObjects()
{
	font_ = Context::Instance().RenderFactoryInstance().MakeFont("gkai00mp.ttf", 16);

	renderElectro_.reset(new RenderElectro);

	RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());
	renderEngine.ClearColor(Color(0.2f, 0.4f, 0.6f, 1));
}

void Electro::DoUpdate(uint32_t pass)
{
	RenderEngine& renderEngine(Context::Instance().RenderFactoryInstance().RenderEngineInstance());

	renderEngine.Clear(RenderEngine::CBM_Color | RenderEngine::CBM_Depth);

	renderElectro_->AddToRenderQueue();

	std::wostringstream stream;
	stream << renderEngine.CurRenderTarget()->FPS();

	font_->RenderText(0, 0, Color(1, 1, 0, 1), L"����Ч��");
	font_->RenderText(0, 18, Color(1, 1, 0, 1), stream.str());
}
