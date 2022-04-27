set PResolutionFormula { 0.0 }
set CtgThetaResolutionFormula { 0.0 }
set PhiResolutionFormula { 0.0 }
# taken from https://cds.cern.ch/record/331063/files/ATLAS-TDR-4-Volume-I.pdf Table 4-2
set D0ResolutionFormula {
    ( abs(eta) >= 0.0 && abs(eta) <= 0.25 ) * (10.9*10.9 + 57.2/pt * 10.9 + 57.2/pt*57.2/pt )*1e-6 +\
    ( abs(eta) > 0.25 && abs(eta) <= 0.5 ) * (10.9*10.9 + 59./pt * 10.9 + 59./pt*59./pt )*1e-6 +\
    ( abs(eta) > 0.5 && abs(eta) <= 0.75 ) * (10.9*10.9 + 62.6/pt * 10.9 + 62.6/pt*62.6/pt )*1e-6 +\
    ( abs(eta) > 0.75 && abs(eta) <= 1 ) * (11*11 + 68.3/pt * 11. + 68.3/pt*68.3/pt )*1e-6 +\
    ( abs(eta) > 1. && abs(eta) <= 1.25 ) * (10.5*10.5 + 75./pt * 10.5 + 75./pt*75.2/pt )*1e-6 +\
    ( abs(eta) > 1.25 && abs(eta) <= 1.5 ) * (10.2*10.2 + 83.4/pt * 10.2 + 83.4/pt*83.4/pt )*1e-6 +\
    ( abs(eta) > 1.5 && abs(eta) <= 1.75 ) * (9.9*9.9 + 93.2/pt * 9.9 + 93.2/pt*93.2/pt )*1e-6 +\
    ( abs(eta) > 1.75 && abs(eta) <= 2 ) * (9.6*9.6 + 104.6/pt * 9.6 + 104.6/pt*104.6/pt )*1e-6 +\
    ( abs(eta) > 2 && abs(eta) <= 2.25 ) * (10.2*10.2 + 118.6/pt * 10.2 + 118.6/pt*118.6/pt )*1e-6 +\
    ( abs(eta) > 2.25 && abs(eta) <= 2.5 ) * (9.6*9.6 + 135.5/pt * 9.6 + (135.5/pt*135.5/pt) )*1e-6
    }
set DZResolutionFormula {
    ( abs(eta) >= 0.0 && abs(eta) <= 0.25 ) * (81.9*81.9 + 85.7/pt * 81.9 + 85.7/pt*85.7/pt )*1e-6 / (cos(atan(1/ctgTheta))/ctgTheta) +\
    ( abs(eta) > 0.25 && abs(eta) <= 0.5 ) * (69.7*69.7 +78.1/pt * 69.7 + 78.1/pt*78.1/pt )*1e-6 / (cos(atan(1/ctgTheta))/ctgTheta) +\
    ( abs(eta) > 0.5 && abs(eta) <= 0.75 ) * (59.1*59.1 + 75.5/pt * 59.1 + 75.5/pt*75.5/pt )*1e-6 / (cos(atan(1/ctgTheta))/ctgTheta) +\
    ( abs(eta) > 0.75 && abs(eta) <= 1 ) * (49.8*49.8 + 76.2/pt * 49.8 + 76.2/pt*76.2/pt )*1e-6 / (cos(atan(1/ctgTheta))/ctgTheta) +\
    ( abs(eta) > 1. && abs(eta) <= 1.25 ) * (42.3*42.3 + 79.1/pt * 42.3 + 79.1/pt*79.1/pt )*1e-6 / (cos(atan(1/ctgTheta))/ctgTheta) +\
    ( abs(eta) > 1.25 && abs(eta) <= 1.5 ) * (38.*38. + 83.7/pt * 38. + 83.7/pt*83.7/pt )*1e-6 / (cos(atan(1/ctgTheta))/ctgTheta) +\
    ( abs(eta) > 1.5 && abs(eta) <= 1.75 ) * (33.6*33.6 + 91.7/pt * 33.6 + 91.7/pt*91.7/pt )*1e-6 / (cos(atan(1/ctgTheta))/ctgTheta) +\
    ( abs(eta) > 1.75 && abs(eta) <= 2 ) * (29.4*29.4 + 103.6/pt * 29.4 + 103.6/pt*103.6/pt )*1e-6 / (cos(atan(1/ctgTheta))/ctgTheta) +\
    ( abs(eta) > 2 && abs(eta) <= 2.25 ) * (25.6*25.6 + 119.1/pt * 25.6 + 118.1/pt*119.1/pt )*1e-6 / (cos(atan(1/ctgTheta))/ctgTheta) +\
    ( abs(eta) > 2.25 && abs(eta) <= 2.5 ) * (22.3*22.3 + 135.8/pt * 22.3+ 135.8/pt*135.8/pt )*1e-6
   }