#include "driver.h"
#include "module_cell/module_neighbor/sltk_atom_arrange.h"
#include "module_hamilt_pw/hamilt_pwdft/global.h"
#include "module_io/input.h"
#include "module_io/print_info.h"
#include "module_io/winput.h"
#include "module_md/run_md.h"
#include "module_io/para_json.h"

/**
 * @brief This is the driver function which defines the workflow of ABACUS calculations.
 * It relies on the class Esolver, which is a class that organizes workflows of single point calculations.
 * 
 * For calculations involving change of configuration (lattice parameter & ionic motion),
 * this driver calls Esolver::Run and the configuration-changing subroutine in a alternating manner.
 * 
 * Information is passed between the two subroutines by class UnitCell
 * 
 * Esolver::Run takes in a configuration and provides force and stress, 
 * the configuration-changing subroutine takes force and stress and updates the configuration
 */
void Driver::driver_run(void)
{
    ModuleBase::TITLE("Driver", "driver_line");
    ModuleBase::timer::tick("Driver", "driver_line");

    //! 1: initialize the ESolver 
    ModuleESolver::ESolver *p_esolver = nullptr;
    ModuleESolver::init_esolver(p_esolver);

    //! 2: setup cell and atom information
#ifndef __LCAO
    if(GlobalV::BASIS_TYPE == "lcao_in_pw" || GlobalV::BASIS_TYPE == "lcao")
    {
        ModuleBase::WARNING_QUIT("driver","to use LCAO basis, compile with __LCAO");
    }
#endif
    GlobalC::ucell.setup_cell(GlobalV::stru_file, GlobalV::ofs_running);

    //! 3: for these two types of calculations
    // nothing else need to be initialized
    if(GlobalV::CALCULATION == "test_neighbour" 
    || GlobalV::CALCULATION == "test_memory")
    {
        p_esolver->run(0, GlobalC::ucell);
        ModuleBase::QUIT();
    }

    //! 4: initialize Esolver and fill json-structure 
    p_esolver->init(INPUT, GlobalC::ucell);


#ifdef __RAPIDJSON
    Json::gen_stru_wrapper(&GlobalC::ucell);
#endif

    //! 5: md or relax calculations 
    if(GlobalV::CALCULATION == "md")
    {
        Run_MD::md_line(GlobalC::ucell, p_esolver, INPUT.mdp);
    }
    else //! scf; cell relaxation; nscf; etc
    {
        if (GlobalV::precision_flag == "single")
        {
            Relax_Driver<float, psi::DEVICE_CPU> rl_driver;
            rl_driver.relax_driver(p_esolver);
        }
        else
        {
            Relax_Driver<double, psi::DEVICE_CPU> rl_driver;
            rl_driver.relax_driver(p_esolver);
        }
    }

    //! 6: clean up esolver
    p_esolver->post_process();
    ModuleESolver::clean_esolver(p_esolver);

    ModuleBase::timer::tick("Driver", "driver_line");
    return;
}
