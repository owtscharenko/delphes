#######################################
# Order of execution of various modules
#######################################

set ExecutionPath {

  PileUpMerger
  ParticlePropagator

  ChargedHadronTrackingEfficiency
  ElectronTrackingEfficiency
  MuonTrackingEfficiency

  ChargedHadronMomentumSmearing
  ElectronMomentumSmearing
  MuonMomentumSmearing

  TrackMerger

  VertexFinder

  TreeWriter
}

###############
# PileUp Merger
###############

module PileUpMerger PileUpMerger {
  set InputArray Delphes/stableParticles

  set ParticleOutputArray stableParticles
  set VertexOutputArray vertices

  # pre-generated minbias input file
  set PileUpFile MinBias.pileup
  
  #set PileUpFile /media/niko/big_data/delphes_output/main42_beam_spot_pileup_500.pileup

  # average expected pile up
  set MeanPileUp 50

   # maximum spread in the beam direction in m
  set ZVertexSpread 0.04

  # maximum spread in time in s
  set TVertexSpread 800E-12

  # vertex smearing formula f(z,t) (z,t need to be respectively given in m,s)
  set VertexDistributionFormula {exp(-(t^2/160e-12^2/2))*exp(-(z^2/0.053^2/2))}


}

#################################
# Propagate particles in cylinder
#################################

module ParticlePropagator ParticlePropagator {
  set InputArray PileUpMerger/stableParticles

  set OutputArray stableParticles
  set ChargedHadronOutputArray chargedHadrons
  set ElectronOutputArray electrons
  set MuonOutputArray muons

  # radius of the magnetic field coverage, in m
  set Radius 1.15
  # half-length of the magnetic field coverage, in m
  set HalfLength 3.51

  # magnetic field
  set Bz 2.0
}

####################################
# Charged hadron tracking efficiency
####################################

module Efficiency ChargedHadronTrackingEfficiency {
  set InputArray ParticlePropagator/chargedHadrons
  set OutputArray chargedHadrons

  # add EfficiencyFormula {efficiency formula as a function of eta and pt}

  # tracking efficiency formula for charged hadrons
  set EfficiencyFormula {                                                    (pt <= 0.1)   * (0.00) +
                                           (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.70) +
                                           (abs(eta) <= 1.5) * (pt > 1.0)                  * (0.95) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.60) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0)                  * (0.85) +
                         (abs(eta) > 2.5)                                                  * (0.00)}
}

##############################
# Electron tracking efficiency
##############################

module Efficiency ElectronTrackingEfficiency {
  set InputArray ParticlePropagator/electrons
  set OutputArray electrons

  # set EfficiencyFormula {efficiency formula as a function of eta and pt}

  # tracking efficiency formula for electrons
  set EfficiencyFormula {                                                    (pt <= 0.1)   * (0.00) +
                                           (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.73) +
                                           (abs(eta) <= 1.5) * (pt > 1.0   && pt <= 1.0e2) * (0.95) +
                                           (abs(eta) <= 1.5) * (pt > 1.0e2)                * (0.99) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.50) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0   && pt <= 1.0e2) * (0.83) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0e2)                * (0.90) +
                         (abs(eta) > 2.5)                                                  * (0.00)}
}

##########################
# Muon tracking efficiency
##########################

module Efficiency MuonTrackingEfficiency {
  set InputArray ParticlePropagator/muons
  set OutputArray muons

  # set EfficiencyFormula {efficiency formula as a function of eta and pt}

  # tracking efficiency formula for muons
  set EfficiencyFormula {                                                    (pt <= 0.1)   * (0.00) +
                                           (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.75) +
                                           (abs(eta) <= 1.5) * (pt > 1.0)                  * (0.99) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.70) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0)                  * (0.98) +
                         (abs(eta) > 2.5)                                                  * (0.00)}
}

########################################
# Momentum resolution for charged tracks
########################################

module MomentumSmearing ChargedHadronMomentumSmearing {
  set InputArray ChargedHadronTrackingEfficiency/chargedHadrons
  set OutputArray chargedHadrons

  # set ResolutionFormula {resolution formula as a function of eta and pt}

  # resolution formula for charged hadrons
  # based on arXiv:1405.6569
  set ResolutionFormula {                  (abs(eta) <= 0.5) * (pt > 0.1) * sqrt(0.06^2 + pt^2*1.3e-3^2) +
                         (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 0.1) * sqrt(0.10^2 + pt^2*1.7e-3^2) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1) * sqrt(0.25^2 + pt^2*3.1e-3^2)}
}

###################################
# Momentum resolution for electrons
###################################

module MomentumSmearing ElectronMomentumSmearing {
  set InputArray ElectronTrackingEfficiency/electrons
  set OutputArray electrons

  # set ResolutionFormula {resolution formula as a function of eta and energy}

  # resolution formula for electrons
  # based on arXiv:1405.6569
  set ResolutionFormula {                  (abs(eta) <= 0.5) * (pt > 0.1) * sqrt(0.03^2 + pt^2*1.3e-3^2) +
                         (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 0.1) * sqrt(0.05^2 + pt^2*1.7e-3^2) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1) * sqrt(0.15^2 + pt^2*3.1e-3^2)}
}

###############################
# Momentum resolution for muons
###############################

module MomentumSmearing MuonMomentumSmearing {
  set InputArray MuonTrackingEfficiency/muons
  set OutputArray muons

  # set ResolutionFormula {resolution formula as a function of eta and pt}

  # resolution formula for muons
  set ResolutionFormula {                  (abs(eta) <= 0.5) * (pt > 0.1) * sqrt(0.01^2 + pt^2*1.0e-4^2) +
                         (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 0.1) * sqrt(0.015^2 + pt^2*1.5e-4^2) +
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1) * sqrt(0.025^2 + pt^2*3.5e-4^2)}
}

##############
# Track merger
##############

module Merger TrackMerger {
# add InputArray InputArray
  add InputArray ChargedHadronMomentumSmearing/chargedHadrons
  add InputArray ElectronMomentumSmearing/electrons
  add InputArray MuonMomentumSmearing/muons
  set OutputArray tracks
}

################################                                                                    
# Track impact parameter smearing                                                                   
################################                                                                    

module TrackSmearing TrackSmearing {
  set InputArray TrackMerger/tracks
#  set BeamSpotInputArray BeamSpotFilter/beamSpotParticle
  set OutputArray tracks
#  set ApplyToPileUp true

  # magnetic field
  set Bz 4.0

  #source trackResolution.tcl
}

##################################
# Primary vertex reconstruction
##################################


module VertexFinder VertexFinder {

  # set InputArray TrackSmearing/tracks
#  set InputArray TimeSmearing/tracks
 set InputArray TrackMerger/tracks

  set OutputArray tracks
  set VertexOutputArray vertices

  set MinPT 1.0
  set MinNDF 4
  set SeedMinPT 1.0

  set Sigma     3.0
  set MaxEta    10.0
  set GrowSeeds 1

}

##################
# ROOT tree writer
##################

# tracks, towers and eflow objects are not stored by default in the output.
# if needed (for jet constituent or other studies), uncomment the relevant
# "add Branch ..." lines.

module TreeWriter TreeWriter {
# add Branch InputArray BranchName BranchClass
  add Branch Delphes/allParticles Particle GenParticle

  add Branch PileUpMerger/vertices GenVertex Vertex

  add Branch TrackMerger/tracks Track Track

  add Branch PileUpMerger/vertices Vertex Vertex
  add Branch VertexFinder/vertices FitVertex Vertex

}
