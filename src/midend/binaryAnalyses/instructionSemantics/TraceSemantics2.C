#include "sage3basic.h"
#include "TraceSemantics2.h"
#include "AsmUnparser_compat.h"

namespace rose {
namespace BinaryAnalysis {
namespace InstructionSemantics2 {
namespace TraceSemantics {

void
RiscOperators::linePrefix() {
    if (stream_) {
        const char *sep = "";
        if (subdomain_) {
            stream_ <<subdomain_->get_name() <<"@" <<subdomain_.get();
            sep = " ";
        }
        if (SgAsmInstruction *insn = get_insn()) {
            stream_ <<sep <<"insn@" <<StringUtility::addrToString(insn->get_address()) <<"[" <<(nInsns_-1) <<"]";
            sep = " ";
        }
        if (*sep)
            stream_ <<": ";
    }
}

std::string
RiscOperators::toString(const BaseSemantics::SValuePtr &a)
{
    // FIXME: if there's a way to determine if "a" is not a subclass of the subdomain's protoval class then we could also spit
    // out a warning. [Robb P. Matzke 2013-09-13]
    std::ostringstream ss;
    if (a==NULL) {
        ss <<"NULL";
    } else if (0==a->get_width()) {
        ss <<"PROTOVAL";
    } else {
        ss <<*a;
    }
    return ss.str();
}

void
RiscOperators::check_equal_widths(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    if (a!=NULL && b!=NULL && a->get_width()!=b->get_width())
        stream_ <<"value width violation; see documentation for this RISC operator!\n";
}

const BaseSemantics::SValuePtr &
RiscOperators::check_width(const BaseSemantics::SValuePtr &a, size_t nbits, const std::string &what)
{
    if (a==NULL || a->get_width()!=nbits)
        stream_ <<"expected " <<(what.empty()?std::string("result"):what)
                <<" to be " <<nbits <<" bits wide; see documentation for this RISC operator!\n";
    return a;
}

std::string
RiscOperators::register_name(const RegisterDescriptor &a) 
{
    BaseSemantics::StatePtr state = subdomain_->get_state();
    BaseSemantics::RegisterStatePtr regstate;
    if (state!=NULL)
        regstate = state->get_register_state();
    RegisterNames regnames(regstate!=NULL ? regstate->get_register_dictionary() : NULL);
    return regnames(a);
}

void
RiscOperators::before(const std::string &operator_name)
{
    linePrefix();
    stream_ <<operator_name <<"()";
    checkSubdomain();
}

void
RiscOperators::before(const std::string &operator_name, const RegisterDescriptor &a)
{
    checkSubdomain();
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<register_name(a) <<")";
}

void
RiscOperators::before(const std::string &operator_name, const RegisterDescriptor &a, const BaseSemantics::SValuePtr &b)
{
    checkSubdomain();
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<register_name(a) <<", " <<toString(b) <<")";
}

void
RiscOperators::before(const std::string &operator_name, const RegisterDescriptor &a, const BaseSemantics::SValuePtr &b,
                      const BaseSemantics::SValuePtr &c, size_t d)
{
    checkSubdomain();
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<register_name(a) <<", " <<toString(b) <<", " <<toString(c) <<", " <<d <<")";
}

void
RiscOperators::before(const std::string &operator_name, const RegisterDescriptor &a, const BaseSemantics::SValuePtr &b,
                      const BaseSemantics::SValuePtr &c, const BaseSemantics::SValuePtr &d)
{
    checkSubdomain();
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<register_name(a) <<", " <<toString(b) <<", " <<toString(c) <<", "
                         <<toString(d) <<")";
}

void
RiscOperators::before(const std::string &operator_name, SgAsmInstruction *insn, bool showAddress)
{
    linePrefix();
    if (showAddress) {
        SAWYER_MESG(stream_) <<operator_name <<"(" <<StringUtility::trim(unparseInstructionWithAddress(insn)) <<")";
    } else {
        SAWYER_MESG(stream_) <<operator_name <<"(" <<StringUtility::trim(unparseInstruction(insn)) <<")";
    }
    checkSubdomain();
}

void
RiscOperators::before(const std::string &operator_name, size_t a)
{
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<a <<")";
    checkSubdomain();
}

void
RiscOperators::before(const std::string &operator_name, size_t a, uint64_t b)
{
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<a <<", " <<b <<")";
    checkSubdomain();
}

void
RiscOperators::before(const std::string &operator_name, const BaseSemantics::SValuePtr &a)
{
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<toString(a) <<")";
    checkSubdomain();
}

void
RiscOperators::before(const std::string &operator_name, const BaseSemantics::SValuePtr &a, size_t b)
{
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<toString(a) <<", " <<b <<")";
    checkSubdomain();
}

void
RiscOperators::before(const std::string &operator_name, const BaseSemantics::SValuePtr &a, size_t b, size_t c)
{
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<toString(a) <<", " <<b <<", " <<c <<")";
    checkSubdomain();
}

void
RiscOperators::before(const std::string &operator_name, const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<toString(a) <<", " <<toString(b) <<")";
    checkSubdomain();
}

void
RiscOperators::before(const std::string &operator_name, const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b,
                      const BaseSemantics::SValuePtr &c)
{
    linePrefix();
    SAWYER_MESG(stream_) <<operator_name <<"(" <<toString(a) <<", " <<toString(b) <<", " <<toString(c) <<")";
    checkSubdomain();
}

void
RiscOperators::after()
{
    stream_ <<"\n";
}

const BaseSemantics::SValuePtr &
RiscOperators::after(const BaseSemantics::SValuePtr &retval)
{
    SAWYER_MESG(stream_) <<" = " <<toString(retval) <<"\n";
    return retval;
}

const BaseSemantics::SValuePtr &
RiscOperators::after(const BaseSemantics::SValuePtr &retval, const BaseSemantics::SValuePtr &ret2)
{
    SAWYER_MESG(stream_) <<" = " <<toString(retval) <<"\n";
    linePrefix();
    SAWYER_MESG(stream_) <<"also returns: " <<toString(ret2) <<"\n";
    return retval;
}

void
RiscOperators::after(const BaseSemantics::Exception &e)
{
    SAWYER_MESG(stream_) <<" = Exception(" <<e.what() <<")\n";
}

void
RiscOperators::after_exception()
{
    stream_ <<" = <Exception>\n";
}

BaseSemantics::SValuePtr
RiscOperators::get_protoval() const
{
    checkSubdomain();
    return subdomain_->get_protoval();
}

void
RiscOperators::set_solver(SMTSolver *solver)
{
    checkSubdomain();
    subdomain_->set_solver(solver);
}

SMTSolver *
RiscOperators::get_solver() const
{
    checkSubdomain();
    return subdomain_->get_solver();
}

BaseSemantics::StatePtr
RiscOperators::get_state() const
{
    checkSubdomain();
    return subdomain_->get_state();
}

void
RiscOperators::set_state(const BaseSemantics::StatePtr &state)
{
    checkSubdomain();
    subdomain_->set_state(state);
}

void
RiscOperators::print(std::ostream &stream, BaseSemantics::Formatter &fmt) const
{
    checkSubdomain();
    subdomain_->print(stream, fmt);
}

size_t
RiscOperators::get_ninsns() const
{
    checkSubdomain();
    return subdomain_->get_ninsns();
}

void
RiscOperators::set_ninsns(size_t n)
{
    checkSubdomain();
    subdomain_->set_ninsns(n);
}

SgAsmInstruction *
RiscOperators::get_insn() const
{
    checkSubdomain();
    return subdomain_->get_insn();
}

void
RiscOperators::startInstruction(SgAsmInstruction *insn)
{
    ++nInsns_;
    cur_insn = insn;
    before("startInstruction", insn, true /*show address*/);
    try {
        subdomain_->startInstruction(insn);
        after();
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

void
RiscOperators::finishInstruction(SgAsmInstruction *insn)
{
    before("finishInstruction", insn, false /*hide address*/); // address is part of prefix
    try {
        subdomain_->finishInstruction(insn);
        after();
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::undefined_(size_t nbits)
{
    before("undefined_", nbits);
    try {
        return check_width(after(subdomain_->undefined_(nbits)), nbits);
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::number_(size_t nbits, uint64_t value)
{
    before("number_", nbits, value);
    try {
        return check_width(after(subdomain_->number_(nbits, value)), nbits);
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::boolean_(bool value)
{
    before("boolean_", value);
    try {
        return check_width(after(subdomain_->boolean_(value)), 1);
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::filterCallTarget(const BaseSemantics::SValuePtr &a)
{
    before("filterCallTarget", a);
    try {
        return check_width(after(subdomain_->filterCallTarget(a)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::filterReturnTarget(const BaseSemantics::SValuePtr &a)
{
    before("filterReturnTarget", a);
    try {
        return check_width(after(subdomain_->filterReturnTarget(a)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::filterIndirectJumpTarget(const BaseSemantics::SValuePtr &a)
{
    before("filterIndirectJumpTarget", a);
    try {
        return check_width(after(subdomain_->filterIndirectJumpTarget(a)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

void
RiscOperators::hlt()
{
    before("hlt");
    try {
        subdomain_->hlt();
        after();
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

void
RiscOperators::cpuid()
{
    before("cpuid");
    try {
        subdomain_->cpuid();
        after();
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::rdtsc()
{
    before("rdtsc");
    try {
        return check_width(after(subdomain_->rdtsc()), 64);
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::and_(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("and_", a, b);
    try {
        check_equal_widths(a, b);
        return check_width(after(subdomain_->and_(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::or_(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("or_", a, b);
    try {
        check_equal_widths(a, b);
        return check_width(after(subdomain_->or_(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::xor_(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("xor_", a, b);
    try {
        check_equal_widths(a, b);
        return check_width(after(subdomain_->xor_(a, b)), a->get_width());;
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::invert(const BaseSemantics::SValuePtr &a)
{
    before("invert", a);
    try {
        return check_width(after(subdomain_->invert(a)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::extract(const BaseSemantics::SValuePtr &a, size_t b, size_t c)
{
    before("extract", a, b, c);
    try {
        return check_width(after(subdomain_->extract(a, b, c)), c-b);
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::concat(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("concat", a, b);
    try {
        return check_width(after(subdomain_->concat(a, b)), a->get_width()+b->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::leastSignificantSetBit(const BaseSemantics::SValuePtr &a)
{
    before("leastSignificantSetBit", a);
    try {
        return check_width(after(subdomain_->leastSignificantSetBit(a)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::mostSignificantSetBit(const BaseSemantics::SValuePtr &a)
{
    before("mostSignificantSetBit", a);
    try {
        return check_width(after(subdomain_->mostSignificantSetBit(a)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::rotateLeft(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("rotateLeft", a, b);
    try {
        return check_width(after(subdomain_->rotateLeft(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::rotateRight(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("rotateRight", a, b);
    try {
        return check_width(after(subdomain_->rotateRight(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::shiftLeft(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("shiftLeft", a, b);
    try {
        return check_width(after(subdomain_->shiftLeft(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::shiftRight(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("shiftRight", a, b);
    try {
        return check_width(after(subdomain_->shiftRight(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::shiftRightArithmetic(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("shiftRightArithmetic", a, b);
    try {
        return check_width(after(subdomain_->shiftRightArithmetic(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::equalToZero(const BaseSemantics::SValuePtr &a)
{
    before("equalToZero", a);
    try {
        return check_width(after(subdomain_->equalToZero(a)), 1);
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::ite(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b, const BaseSemantics::SValuePtr &c)
{
    before("ite", a, b, c);
    try {
        check_equal_widths(b, c);
        return check_width(after(subdomain_->ite(a, b, c)), b->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::unsignedExtend(const BaseSemantics::SValuePtr &a, size_t b)
{
    before("unsignedExtend", a, b);
    try {
        return check_width(after(subdomain_->unsignedExtend(a, b)), b);
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::signExtend(const BaseSemantics::SValuePtr &a, size_t b)
{
    before("signExtend", a, b);
    try {
        return check_width(after(subdomain_->signExtend(a, b)), b);
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::add(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("add", a, b);
    try {
        check_equal_widths(a, b);
        return check_width(after(subdomain_->add(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::addWithCarries(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b,
                              const BaseSemantics::SValuePtr &c, BaseSemantics::SValuePtr &d/*out*/)
{
    before("addWithCarries", a, b, c);
    try {
        check_equal_widths(a, b);
        check_width(c, 1);
        BaseSemantics::SValuePtr retval = subdomain_->addWithCarries(a, b, c, d);
        after(retval, d);
        check_width(retval, a->get_width());
        check_width(d, a->get_width());
        return retval;
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::negate(const BaseSemantics::SValuePtr &a)
{
    before("negate", a);
    try {
        return check_width(after(subdomain_->negate(a)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::signedDivide(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("signedDivide", a, b);
    try {
        return check_width(after(subdomain_->signedDivide(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::signedModulo(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("signedModulo", a, b);
    try {
        return check_width(after(subdomain_->signedModulo(a, b)), b->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::signedMultiply(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("signedMultiply", a, b);
    try {
        return check_width(after(subdomain_->signedMultiply(a, b)), a->get_width()+b->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::unsignedDivide(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("unsignedDivide", a, b);
    try {
        return check_width(after(subdomain_->unsignedDivide(a, b)), a->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::unsignedModulo(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("unsignedModulo", a, b);
    try {
        return check_width(after(subdomain_->unsignedModulo(a, b)), b->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::unsignedMultiply(const BaseSemantics::SValuePtr &a, const BaseSemantics::SValuePtr &b)
{
    before("unsignedMultiply", a, b);
    try {
        return check_width(after(subdomain_->unsignedMultiply(a, b)), a->get_width()+b->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

void
RiscOperators::interrupt(int a, int b)
{
    before("interrupt", a, b);
    try {
        subdomain_->interrupt(a, b);
        after();
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::readRegister(const RegisterDescriptor &a)
{
    before("readRegister", a);
    try {
        return check_width(after(subdomain_->readRegister(a)), a.get_nbits());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

void
RiscOperators::writeRegister(const RegisterDescriptor &a, const BaseSemantics::SValuePtr &b)
{
    before("writeRegister", a, b);
    try {
        subdomain_->writeRegister(a, b);
        after();
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

BaseSemantics::SValuePtr
RiscOperators::readMemory(const RegisterDescriptor &a, const BaseSemantics::SValuePtr &b, const BaseSemantics::SValuePtr &c,
                          const BaseSemantics::SValuePtr &d)
{
    before("readMemory", a, b, c, d);
    try {
        return check_width(after(subdomain_->readMemory(a, b, c, d)), c->get_width());
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

void
RiscOperators::writeMemory(const RegisterDescriptor &a, const BaseSemantics::SValuePtr &b, const BaseSemantics::SValuePtr &c,
                           const BaseSemantics::SValuePtr &d)
{
    before("writeMemory", a, b, c, d);
    try {
        subdomain_->writeMemory(a, b, c, d);
        after();
    } catch (const BaseSemantics::Exception &e) {
        after(e);
        throw;
    } catch (...) {
        after_exception();
        throw;
    }
}

} // namespace
} // namespace
} // namespace
} // namespace
