#pragma once
#include <Enigma.hpp>
using namespace Enigma;

class DecryptTextScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit DecryptTextScene(const std::unordered_map<String, ImFont*>& fonts);
	virtual ~DecryptTextScene() = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Event& event) override;
	void OnDestroy() override;

private:
	const std::unordered_map<String, ImFont*>& m_fonts;
};

