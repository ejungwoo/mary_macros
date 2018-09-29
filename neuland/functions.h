const auto kmpi  = 139.57018;
const auto kmp   = 938.2720813;
const auto kmd   = 1875.612762;
const auto kmt   = 2808.921112;
const auto kmhe3 = 2808.39132;
const auto kmal  = 3727.379378;
const auto kme   = 0.5109989461;

const auto tof0 = 29.;
const auto df = 1000.; // 10 m ?

/**
 * calculating dedx difference from reference to evaluated function value.
 * This function is for minimizing dedx function to calculate mass
 */
Double_t fddedx(Double_t *x, Double_t *p);

/**
 *  dE/dx function - Modified Bethe-Bloch formula
 */
Double_t fdedx(Double_t Z, Double_t mass, Double_t *x, Double_t *p);

Double_t fdedx_pim (Double_t *x, Double_t *p) { return fdedx(-1, kmpi,  x, p); }
Double_t fdedx_pi  (Double_t *x, Double_t *p) { return fdedx( 1, kmpi,  x, p); }
Double_t fdedx_p   (Double_t *x, Double_t *p) { return fdedx( 1, kmp,   x, p); }
Double_t fdedx_d   (Double_t *x, Double_t *p) { return fdedx( 1, kmd,   x, p); }
Double_t fdedx_t   (Double_t *x, Double_t *p) { return fdedx( 1, kmt,   x, p); }
Double_t fdedx_he3 (Double_t *x, Double_t *p) { return fdedx( 2, kmhe3, x, p); }
Double_t fdedx_al  (Double_t *x, Double_t *p) { return fdedx( 2, kmal,  x, p); }

/**
 *
 */
Double_t fenergy(Double_t *x, Double_t *p);

Double_t fddedx(Double_t *x, Double_t *p) {
  auto mass = x[0];
  // p[2] = tof
  // p[3] = Z
  // p[4] = dedx
  auto ZM = 17.2/37.6;
  auto I2 = 12.*p[3]+7.; I2 = I2*I2;
  auto v1 = df/(p[2]-tof0);
  auto g1 = 1./TMath::Sqrt(1.-v1*v1);
  auto p0 = g1*mass*v1;
  auto b2 = p0*p0/(p0*p0+mass*mass);
  auto g2 = 1./(1.-b2);
  auto Wx = 2*kme*b2*g2/((kme+mass)*(kme+mass)+2*kme*mass*(TMath::Sqrt(g2)-1));
  auto ddedx = p[4]-p[0]*ZM*p[3]*p[3]/b2*(0.5*TMath::Log(2*kme*b2*g2*Wx/I2)-b2-p[1]);
  return ddedx;
}

Double_t fdedx(Double_t Z, Double_t mass, Double_t *x, Double_t *p) {
  auto ZM = 17.2/37.6;
  auto I2 = 12.*p[3]+7.; I2 = I2*I2;
  //auto p0 = mass*(x[0]-tof0);
  auto v1 = df/(x[0]-tof0)/TMath::C();
  auto g1 = 1./TMath::Sqrt(1.-v1*v1);
  auto p0 = g1*mass*v1;
  auto b2 = p0*p0/(p0*p0+mass*mass);
  auto g2 = 1./(1.-b2);
  auto Wx = 2*kme*b2*g2/((kme/mass)*(kme/mass)+2*kme*mass*(TMath::Sqrt(g2)-1));
  auto dedx = p[0]*ZM*Z*Z/b2*(0.5*TMath::Log(2*kme*b2*g2*Wx/I2)-b2-p[1]);
  return dedx;
}

//Double_t fenergy(Double_t mass, Double_t *x, Double_t *p) {
//  auto p0 = mass*(x[0]-tof0);
//  return p[0]*TMath::Sqrt(p0*p0+mass*mass);
//}
